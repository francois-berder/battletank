#include "Host.hpp"

Host::Host():
m_thread(),
m_listener(),
m_selector(),
m_players(),
m_playersToRemove(),
m_running(false)
{

}

void Host::start()
{
    if(m_running)
        return;

    m_running = true;

    std::thread thread(&Host::run, this);
    m_thread.swap(thread);
}

void Host::stop()
{
    if(!m_running)
        return;

    m_running = false;
    m_thread.join();
}

void Host::run()
{
    m_listener.listen(9999);
    m_selector.add(m_listener);

    while(m_running)
    {
        if(m_selector.wait(sf::milliseconds(100)))
        {
            if(m_selector.isReady(m_listener))
                handleIncomingClient();
            else
            {
                QMap<QString, sf::TcpSocket*>::iterator itor;
                for(itor = m_players.begin();
                    itor != m_players.end();
                    ++itor)
                {
                    if(m_selector.isReady(*itor.value()))
                        handleData(itor.key(), *itor.value());
                }
                QList<QString>::iterator itor2;
                for(itor2 = m_playersToRemove.begin();
                    itor2 != m_playersToRemove.end();
                    ++itor2)
                    m_players.remove(*itor2);
                m_playersToRemove.clear();
            }
        }
    }
    m_selector.clear();
    m_listener.close();

    QList<sf::TcpSocket*>::iterator itor;
    for(itor = m_players.values().begin();
        itor != m_players.values().end();
        ++itor)
    {
        (*itor)->disconnect();
        delete (*itor);
    }
    m_players.clear();
    m_playersToRemove.clear();
}

void Host::handleIncomingClient()
{
    sf::TcpSocket *client = new sf::TcpSocket;
    if(m_listener.accept(*client) == sf::Socket::Done)
    {
        sf::Packet packet;
        client->receive(packet);
        std::string cmd;
        packet >> cmd;
        if(cmd == "PING_GAME")
        {
            std::string pseudo;
            packet >> pseudo;
            packet.clear();
            if(m_players.keys().contains(QString::fromStdString(pseudo)))
                packet << "PSEUDO_ALREADY_IN_USE";
            else
                packet << "GAME_EXIST";

            client->send(packet);
            client->disconnect();
            delete client;
        }
        else if(cmd == "JOIN")
        {
            std::string pseudo;
            packet >> pseudo;

            // TODO: check pseudo not already in m_players


            // Send existing players' list
            packet.clear();
            packet << "EXISTING_PLAYERS";
            packet << (unsigned int)m_players.size();
            QMap<QString, sf::TcpSocket*>::iterator itor;
            for(itor = m_players.begin();
                itor != m_players.end();
                ++itor)
                packet << itor.key().toStdString();
            client->send(packet);
            m_players[QString::fromStdString(pseudo)] = client;
            m_selector.add(*client);

            // Send to all players that another player joined
            packet.clear();
            packet << "PLAYER_JOINED";
            packet << pseudo;
            for(itor = m_players.begin();
                itor != m_players.end();
                ++itor)
                itor.value()->send(packet);

        }
        else
        {
            client->disconnect();
            delete client;
        }
    }
    else
        delete client;
}

void Host::handleData(QString pseudo, sf::TcpSocket &socket)
{
    sf::Packet packet;
    socket.receive(packet);
    std::string cmdName;
    packet >> cmdName;
    if(cmdName == "MESSAGE")  // One player wrote a message
    {
        std::string str;
        packet >> str;

        // Send this message to everyone
        packet.clear();
        packet << "MESSAGE";
        packet << pseudo.toStdString();
        packet << str;
        QList<sf::TcpSocket*>::iterator itor;
        for(itor = m_players.values().begin();
            itor != m_players.values().end();
            ++itor)
            (*itor)->send(packet);
    }
    else if(cmdName == "LEAVE")  // One player left
    {
        std::string pseudo;
        packet >> pseudo;
        QString p = QString::fromStdString(pseudo);
        m_selector.remove(*m_players[p]);
        m_players[p]->disconnect();
        delete m_players[p];
        m_playersToRemove.append(p);

        // Send this information to all other players
        packet.clear();
        packet << "PLAYER_LEFT";
        packet << pseudo;
        QMap<QString, sf::TcpSocket*>::iterator itor;
        for(itor = m_players.begin();
            itor != m_players.end();
            ++itor)
            if(itor.key() != p)
                itor.value()->send(packet);
    }
}

