#include "Host.hpp"
#include "Logger.hpp"

Host::Host(GameData &data):
QObject(),
m_thread(),
m_listener(),
m_selector(),
m_sockets(),
m_socketsToRemove(),
m_running(false),
m_gameLaunched(false),
m_data(data)
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
    if(m_thread.joinable())
        m_thread.join();
}

bool Host::isRunning() const
{
    return m_running;
}

void Host::launchGame()
{
    if(m_gameLaunched)
        return;

    m_gameLaunched = true;

    sf::Packet packet;
    packet << "LAUNCH_GAME";
    QMap<QString, sf::TcpSocket*>::iterator itor;
    for(itor = m_sockets.begin();
        itor != m_sockets.end();
        ++itor)
        itor.value()->send(packet);

    emit gameLaunched();
}

void Host::run()
{
    if(m_listener.listen(9999) != sf::Socket::Done)
    {
        emit errorListener();
        return;
    }

    m_selector.add(m_listener);
    emit created();

    while(m_running)
    {
        if(m_selector.wait(sf::milliseconds(100)))
        {
            if(m_selector.isReady(m_listener))
                handleIncomingClient();
            else
            {
                QMap<QString, sf::TcpSocket*>::iterator itor;
                for(itor = m_sockets.begin();
                    itor != m_sockets.end();
                    ++itor)
                {
                    if(m_selector.isReady(*itor.value()))
                        handleData(itor.key(), *itor.value());
                }
                QList<QString>::iterator itor2;
                for(itor2 = m_socketsToRemove.begin();
                    itor2 != m_socketsToRemove.end();
                    ++itor2)
                    m_sockets.remove(*itor2);
                m_socketsToRemove.clear();
            }
        }
    }
    m_selector.clear();
    m_listener.close();

    sf::Packet packet;
    packet << "GAME_CANCELLED";
    QMap<QString, sf::TcpSocket*>::iterator itor;
    for(itor = m_sockets.begin();
        itor != m_sockets.end();
        ++itor)
    {
        sf::TcpSocket *socket = itor.value();
        socket->send(packet);
        socket->disconnect();
        delete socket;
    }
    m_sockets.clear();
    m_socketsToRemove.clear();
}

void Host::handleIncomingClient()
{
    sf::TcpSocket *client = new sf::TcpSocket;
    if(m_listener.accept(*client) == sf::Socket::Done)
    {
        sf::Packet packet;
        client->receive(packet);
        std::string cmdName;
        packet >> cmdName;
        if(cmdName == "PING_GAME")
        {
            std::string pseudo;
            packet >> pseudo;
            packet.clear();
            if(m_sockets.keys().contains(QString::fromStdString(pseudo)))
                packet << "PSEUDO_ALREADY_IN_USE";
            else if(m_data.getNbPlayers() == 4)
                packet << "GAME_ALREADY_FULL";
            else if(m_gameLaunched)
                packet << "GAME_ALREADY_STARTED";
            else
                packet << "GAME_EXIST";

            client->send(packet);
            client->disconnect();
            delete client;
        }
        else if(cmdName == "JOIN")
        {
            std::string pseudo;
            packet >> pseudo;
            packet.clear();

            // some check
            if(m_sockets.keys().contains(QString::fromStdString(pseudo)))
                packet << "PSEUDO_ALREADY_IN_USE";
            else if(m_data.getNbPlayers() == 4)
                packet << "GAME_ALREADY_FULL";
            else if(m_gameLaunched)
                packet << "GAME_ALREADY_STARTED";
            else
            {
                // Send existing players' list
                packet << "EXISTING_PLAYERS";
                QMap<QString, bool> players = m_data.getPlayers();
                packet << (unsigned int)players.size();
                QMap<QString, bool>::iterator itor;
                for(itor = players.begin();
                    itor != players.end();
                    ++itor)
                    packet << itor.key().toStdString() << itor.value();
                client->send(packet);

                // Send existing text
                packet.clear();
                packet << "EXISTING_TEXT";
                packet << m_data.getText().toStdString();
                client->send(packet);

                // Register client
                m_sockets[QString::fromStdString(pseudo)] = client;
                m_selector.add(*client);

                // Send to all players that another player joined
                packet.clear();
                packet << "PLAYER_JOINED";
                packet << pseudo;
                QMap<QString, sf::TcpSocket*>::iterator itor2;
                for(itor2 = m_sockets.begin();
                    itor2 != m_sockets.end();
                    ++itor2)
                    itor2.value()->send(packet);
            }
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

/*
 * If we receive data from an unknown player, the host tries to receive the
 * entire command but does not transmit anything to other players.
 */
void Host::handleData(QString pseudo, sf::TcpSocket &socket)
{
    if(!m_data.containsPlayer(pseudo))
        Logger::warning() << "Host receive data from an unknown player (" << pseudo.toStdString() << ")\n";

    sf::Packet packet;
    socket.receive(packet);
    std::string cmdName;
    packet >> cmdName;
    if(cmdName == "MESSAGE")  // One player wrote a message
    {
        std::string str;
        packet >> str;

        if(!m_data.containsPlayer(pseudo))
            return;

        // Send this message to everyone
        packet.clear();
        packet << "MESSAGE";
        packet << pseudo.toStdString();
        packet << str;
        QMap<QString, sf::TcpSocket*>::iterator itor;
        for(itor = m_sockets.begin();
            itor != m_sockets.end();
            ++itor)
                itor.value()->send(packet);
    }
    else if(cmdName == "LEAVE")  // One player left
    {
        std::string pseudoReceived;
        packet >> pseudoReceived;

        if(!m_data.containsPlayer(QString::fromStdString(pseudoReceived)))
            return;

        QString p = QString::fromStdString(pseudoReceived);

        if(p != pseudo)
        {
            Logger::warning() << "invalid LEAVE command received from player: " << pseudo.toStdString() << "\n";
            p = pseudo;
        }

        m_selector.remove(*m_sockets[p]);
        m_sockets[p]->disconnect();
        delete m_sockets[p];
        m_socketsToRemove.append(p);

        // Send this information to all other players
        packet.clear();
        packet << "PLAYER_LEFT";
        packet << pseudoReceived;
        QMap<QString, sf::TcpSocket*>::iterator itor;
        for(itor = m_sockets.begin();
            itor != m_sockets.end();
            ++itor)
            if(itor.key() != p)
                itor.value()->send(packet);
    }
    else if(cmdName == "PLAYER_READY"
         || cmdName == "PLAYER_NOT_READY")
    {
        if(!m_data.containsPlayer(pseudo))
            return;

        // Send this message to everyone
        packet.clear();
        packet << cmdName;
        packet << pseudo.toStdString();
        QMap<QString, sf::TcpSocket*>::iterator itor;
        for(itor = m_sockets.begin();
            itor != m_sockets.end();
            ++itor)
                itor.value()->send(packet);
    }
    else if(cmdName == "ABORT_LAUNCH")
    {
        if(!m_data.containsPlayer(pseudo))
            return;

        if(!m_gameLaunched)
        {
            Logger::warning() << "Player " << pseudo.toStdString() << " tried to abort launch although the game has not even been launched";
            return;
        }

        m_gameLaunched = false;

        // Send this message to everyone
        packet.clear();
        packet << cmdName;
        packet << pseudo.toStdString();
        QMap<QString, sf::TcpSocket*>::iterator itor;
        for(itor = m_sockets.begin();
            itor != m_sockets.end();
            ++itor)
                itor.value()->send(packet);
    }
}

