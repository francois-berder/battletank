#include <sstream>
#include <stdexcept>
#include <chrono>

#include "Client.hpp"
#include "Server.hpp"
#include "Logger.hpp"


Client::Client():
m_controlSocket(),
m_controlThread(),
m_dataSocket(),
m_dataThread(),
m_isConnected(false),
m_id(0),
m_name(),
m_events(),
m_eventsMutex(),
m_toSendEvents(),
m_toSendEventsMutex(),
m_initCommands(),
m_initCommandsMutex()
{
}

Client::~Client()
{
    disconnect();
}



unsigned int Client::connect(const std::string ipAddress, const std::string name)
{
    m_name = name;
    try
    {
        makeHandshake(ipAddress);
    }
    catch(std::exception &e)
    {
        std::stringstream ss;
        ss << "Handshake with server failed." << e.what();
        throw std::runtime_error(ss.str());
    }
    
    // wait 100ms to allow server to create listener
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    unsigned short controlPort = static_cast<unsigned short>(Server::getControlPort() + m_id);
    if(m_controlSocket.connect(ipAddress, controlPort, sf::seconds(2.f)) != sf::Socket::Done)
    {
        std::stringstream ss;
        ss << "Could not connect to " << ipAddress << ":" << controlPort;
        throw std::runtime_error(ss.str());
    }
    m_controlSocket.setBlocking(false);
    
    unsigned short dataPort = static_cast<unsigned short>(Server::getDataPort() + m_id);
    if(m_dataSocket.bind(dataPort) != sf::Socket::Done)
    {
        std::stringstream ss;
        ss << "Could not bind data socket to port " << dataPort;
        throw std::runtime_error(ss.str());
    }
    m_dataSocket.setBlocking(false);

    m_isConnected = true;
    std::thread ctrlThread(&Client::runControl, this);
    m_controlThread.swap(ctrlThread);
    
    std::thread dataThread(&Client::runData, this);
    m_dataThread.swap(dataThread);

    Logger::info() << "Established connection to server, id=" << m_id << '\n';
    
    return m_id;
}


void Client::disconnect()
{
    if(!m_isConnected)
        return;
    
    m_isConnected = false;
    m_controlThread.join();
    m_dataThread.join();
    m_id = 0;
    m_name.clear();
    while(!m_events.empty())
        m_events.pop();

    while(!m_toSendEvents.empty())
        m_toSendEvents.pop();

    Logger::info() << "Disconnected from server.\n";
}

bool Client::pollEvent(NetworkEvent& evt)
{
    std::lock_guard<std::mutex> lock(m_eventsMutex);

    if(m_events.empty())
        return false;
    
    evt = m_events.front();
    m_events.pop();

    return true;
}

void Client::pushEvent(NetworkEvent &evt)
{
    std::lock_guard<std::mutex> lock(m_eventsMutex);
    m_toSendEvents.push(evt);
}

void Client::runControl()
{
    while(m_isConnected)
    {
        sf::Packet packet;
        sf::Socket::Status ret = m_controlSocket.receive(packet);
        if(ret == sf::Socket::NotReady)
            continue;
        if(ret == sf::Socket::Error)
        {
            Logger::warning() << "Could not receive control data\n";
            continue;
        }
        if(ret == sf::Socket::Disconnected)
        {
            Logger::error() << "Disconnected from server because of unknown reason.\n";
            m_controlSocket.disconnect();
            return;
        }
        std::string cmd;
        packet >> cmd;
        if(cmd == "SERVER_EXIT")
        {
            Logger::info() << "Server has exited. Disconnected.\n";
            m_controlSocket.disconnect();
            return;
        }
        else if(cmd == "INIT_WORLD")
        {
            std::lock_guard<std::mutex> lock(m_initCommandsMutex);
            unsigned int nbCmds;
            packet >> nbCmds;
            while(nbCmds > 0)
            {
                std::string str;
                packet >> str;
                m_initCommands.push_back(str);
                --nbCmds;
            }
        }
    }

    // Send exit sequence
    sendExitSequence();
}

void Client::makeHandshake(const std::string &ipAddress)
{
    sf::TcpSocket initSocket;
    unsigned short port = Server::getInitPort();
    if(initSocket.connect(ipAddress, port, sf::seconds(2.f)) != sf::Socket::Done)
    {
        std::stringstream ss;
        ss << "Could not connect to " << ipAddress << ":" << port;
        throw std::runtime_error(ss.str());
    }

    sf::Packet packet;
    packet << "REQUEST_JOIN" << m_name;
    if(initSocket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Could not do handshake with server");
    packet.clear();

    if(initSocket.receive(packet) != sf::Socket::Done)
        throw std::runtime_error("Could not do handshake with server");
    std::string str, name;
    packet >> str >> name >> m_id;
    if(str != "JOIN" || m_name != name)
        throw std::runtime_error("Protocol error from server");
    packet.clear();

    packet << "ACCEPT_JOIN" << m_id;
    if(initSocket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Could not do handshake with server");

    initSocket.disconnect();
}

void Client::sendExitSequence()
{
    m_controlSocket.setBlocking(true);
    sf::Packet packet;
    packet << "REQUEST_QUIT" << m_id;
    m_controlSocket.send(packet);
    packet.clear();
    m_controlSocket.receive(packet);
    std::string str;
    unsigned int clientID;
    packet >> str >> clientID;
    if(str != "QUIT" || clientID != m_id)
        Logger::warning() << "Protocol error while disconnecting from server\n";

    m_controlSocket.disconnect();
}

void Client::runData()
{
    sf::UdpSocket sendSocket;
    sendSocket.setBlocking(false);
    while(m_isConnected)
    {
        sf::Packet packet;
        sf::IpAddress remoteAddress;
        unsigned short remotePort;
        sf::Socket::Status ret = m_dataSocket.receive(packet, remoteAddress, remotePort);
        if(ret == sf::Socket::Error)
            Logger::warning() << "Failed to receive some data.\n";

        if(ret == sf::Socket::Done)
        {
            std::lock_guard<std::mutex> lock(m_eventsMutex);
            NetworkEvent e;
            e.fromPacket(packet);
            m_events.push(e);
        }

        {
            std::lock_guard<std::mutex> lock(m_toSendEventsMutex);
            while(!m_toSendEvents.empty())
            {
                packet.clear();
                NetworkEvent e = m_toSendEvents.front();
                e.toPacket(packet);
                unsigned short port = static_cast<unsigned short>(Server::getDataPort() - m_id);
                sendSocket.send(packet, "localhost", port);
                m_toSendEvents.pop();
            }
        }
    }

    m_dataSocket.unbind();
}

unsigned int Client::getID() const
{
    return m_id;
}

std::list<std::string> Client::getWorld()
{
    m_initCommandsMutex.lock();
    std::size_t size = m_initCommands.size();
    m_initCommandsMutex.unlock();
    while(size == 0)
    {
        m_initCommandsMutex.lock();
        size = m_initCommands.size();
        m_initCommandsMutex.unlock();
    }

    Logger::info() << "Received world.\n";

    m_initCommandsMutex.lock();
    std::list<std::string> initCommands = m_initCommands;
    m_initCommandsMutex.unlock();

    return initCommands;
}

std::string Client::getName() const
{
    return m_name;
}
