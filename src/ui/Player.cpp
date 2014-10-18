#include "Player.hpp"

Player::Player():
QObject(),
m_pseudo(),
m_joinedGame(false),
m_gameCancelled(false),
m_gameLaunchStarted(false),
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
        std::string pseudo;
        packet >> pseudo;
        QString p = QString::fromStdString(pseudo);
        emit playerJoined(p);
        bool isReady;
        packet >> isReady;
        if(isReady)
            emit playerReady(p);
        else
            emit playerNotReady(p);
    }

    // receive existing text
    std::string text;
    packet.clear();
    m_socket.receive(packet);
    packet >> answer;
    if(answer != "EXISTING_TEXT")
    {
        m_socket.disconnect();
        // TODO: emit error
    }
    packet >> text;
    emit existingText(QString::fromStdString(text));


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
    m_gameCancelled = false;
}

void Player::sendMessage(QString message)
{
    sf::Packet packet;
    packet << "MESSAGE";
    packet << message.toStdString();
    m_socket.send(packet);
}

void Player::setReadiness(bool isReady)
{
    sf::Packet packet;
    if(isReady)
        packet << "PLAYER_READY";
    else
        packet << "PLAYER_NOT_READY";
    m_socket.send(packet);
}

void Player::abortLaunch()
{
    if(!m_gameLaunchStarted)
        return;

    sf::Packet packet;
    packet << "ABORT_LAUNCH";
    m_socket.send(packet);
}

void Player::run()
{
    m_selector.add(m_socket);

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
    if(!m_gameCancelled)
    {
        sf::Packet packet;
        packet << "LEAVE";
        packet << m_pseudo.toStdString();
        m_socket.send(packet);
    }
    m_socket.disconnect();
    m_selector.clear();
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
    else if(cmdName == "GAME_CANCELLED")
    {
        m_gameCancelled = true;
        emit gameCancelled();
    }
    else if(cmdName == "PLAYER_READY")
    {
        std::string pseudo;
        packet >> pseudo;
        emit playerReady(QString::fromStdString(pseudo));
    }
    else if(cmdName == "PLAYER_NOT_READY")
    {
        std::string pseudo;
        packet >> pseudo;
        emit playerNotReady(QString::fromStdString(pseudo));
    }
    else if(cmdName == "LAUNCH_GAME")
    {
        m_gameLaunchStarted = true;
        emit gameLaunchStarted();
    }
    else if(cmdName == "ABORT_LAUNCH")
    {
        m_gameLaunchStarted = false;
        std::string pseudo;
        packet >> pseudo;
        emit gameLaunchAborted(QString::fromStdString(pseudo));
    }
}

void Player::setPseudo(QString pseudo)
{
    m_pseudo = pseudo;
}

QString Player::getPseudo() const
{
    return m_pseudo;
}

QString Player::getIPAddress() const
{
    sf::IpAddress ipAddress = sf::IpAddress::getPublicAddress(sf::milliseconds(200));
    if(ipAddress.toString() == "0.0.0.0")
        ipAddress = sf::IpAddress::getLocalAddress();
    if(ipAddress.toString() == "0.0.0.0")
        return "127.0.0.1";
    return ipAddress.toString().c_str();
}
