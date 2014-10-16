#include "Player.hpp"

Player::Player():
QObject(),
m_pseudo(),
m_joinedGame(false),
m_thread(),
m_selector(),
m_socket()
{

}

void Player::join(QString serverAddress)
{
    if(m_joinedGame)
        return;

    m_socket.connect(serverAddress.toStdString(), 9999);
    sf::Packet packet;
    packet << "JOIN";
    packet << m_pseudo.toStdString();
    m_socket.send(packet);

    // receive list of existing players
    packet.clear();
    m_socket.receive(packet);
    std::string answer;
    packet >> answer;
    if(answer != "EXISTING_PLAYERS")
    {
        m_socket.disconnect();
        // TODO: emit error
    }
    unsigned int nbPlayers;
    packet >> nbPlayers;
    for(unsigned int i = 0; i < nbPlayers; ++i)
    {
        std::string name;
        packet >> name;
        emit playerJoined(QString::fromStdString(name));
    }

    m_selector.add(m_socket);
    m_joinedGame = true;
    std::thread thread(&Player::run, this);
    m_thread.swap(thread);
}

void Player::leave()
{
    if(!m_joinedGame)
        return;

    m_joinedGame = false;
    m_thread.join();
    m_selector.clear();
}

void Player::sendMessage(QString message)
{
    sf::Packet packet;
    packet << "MESSAGE";
    packet << message.toStdString();
    m_socket.send(packet);
}

void Player::run()
{
    while(m_joinedGame)
    {
        if(m_selector.wait(sf::milliseconds(100)))
        {
            if(m_selector.isReady(m_socket))
            {
                sf::Packet packet;
                m_socket.receive(packet);
                handleData(packet);
            }
        }
    }

    // Send leave message
    sf::Packet packet;
    packet << "LEAVE";
    packet << m_pseudo.toStdString();
    m_socket.send(packet);
    m_socket.disconnect();
}

void Player::handleData(sf::Packet &packet)
{
    std::string cmdName;
    packet >> cmdName;

    if(cmdName == "MESSAGE")
    {
        std::string pseudo, msg;
        packet >> pseudo >> msg;
        emit receivedMessage(QString::fromStdString(pseudo), QString::fromStdString(msg));
    }
    else if(cmdName == "PLAYER_JOINED")
    {
        std::string pseudo;
        packet >> pseudo;
        emit playerJoined(QString::fromStdString(pseudo));
    }
    else if(cmdName == "PLAYER_LEFT")
    {
        std::string pseudo;
        packet >> pseudo;
        emit playerLeft(QString::fromStdString(pseudo));
    }
}

void Player::setPseudo(QString pseudo)
{
    m_pseudo = pseudo;
}
