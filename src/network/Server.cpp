#include <sstream>
#include <stdexcept>

#include "Server.hpp"
#include "Logger.hpp"
#include "NetworkEvent.hpp"


Server::Server():
m_initThread(),
m_controlThread(),
m_dataThread(),
m_acceptClients(false),
m_control(false),
m_running(false),
m_id(1),
m_clients(),
m_selector(),
m_clientMutex(),
m_sendWorld(false),
m_world()
{
}

Server::~Server()
{
    stopAcceptingClients();
    stop();
}

void Server::startAcceptingClients()
{
    if(m_acceptClients)
        return;

    m_id = 1;
    m_acceptClients = true;
    m_control = true;
    std::thread init(&Server::runInit, this);
    m_initThread.swap(init);
    std::thread control(&Server::runControl, this);
    m_controlThread.swap(control);

    Logger::info() << "Server starts accepting clients.\n";
}

void Server::stopAcceptingClients()
{
    if(!m_acceptClients)
        return;
    
    m_acceptClients = false;
    m_initThread.join();
    m_sendWorld = false;
    m_world.clear();

    Logger::info() << "Server stops accepting clients.\n";
}

void Server::start()
{
    if(m_running)
        return;

    m_running = true;
    std::thread data(&Server::runData, this);
    m_dataThread.swap(data);
    
    Logger::info() << "Server started.\n";
}

void Server::stop()
{
    if(!m_running)
        return;

    m_running = false;
    m_control = false;
    m_controlThread.join();
    m_dataThread.join();
    m_clients.clear();
    m_selector.clear();

    Logger::info() << "Server stopped.\n";
}

void Server::initWorld()
{
    {
        std::lock_guard<std::mutex> lock(m_clientMutex);
        for(auto& c : m_clients)
        {
            std::stringstream ss;
            ss << "a new tank ";
            ss << c.first * 2;
            ss << " ";
            ss << 5;
            m_world.push_back(ss.str());
        }
    }

    m_world.push_back("a new obstacle 10 11");
    m_world.push_back("a new obstacle 10 10");

    m_sendWorld = true;
}

void Server::runInit()
{
    sf::TcpListener listener;
    unsigned short port = Server::getInitPort();
    if(listener.listen(port) != sf::Socket::Done)
    {
        std::stringstream ss;
        ss << "Could not listen on port " << port;
        throw std::runtime_error(ss.str());
    }
    Logger::info() << "Listening for incoming connections on port " << port << ".\n";
    listener.setBlocking(false);    
    while(m_acceptClients)
    {
        sf::TcpSocket initClientSocket;
        sf::Socket::Status ret = listener.accept(initClientSocket);
        if(ret == sf::Socket::NotReady)
            continue;
        if(ret == sf::Socket::Error)
        {
            Logger::warning() << "Failed to accept incoming client.\n";
            continue;
        }
        // TODO: Should handle case: ret == sf::Socket::Disconnected
        try
        {
            makeHandshake(initClientSocket);

        }
        catch(std::exception &e)
        {
            Logger::error() << "Handshake failed with client. " << e.what() << '\n';
        }
        addClient(m_id); // TODO: Handle exceptions
        
        Logger::info() << "New player (id=" << m_id << ") from " << initClientSocket.getRemoteAddress().toString() << '\n';
        initClientSocket.disconnect();

        ++m_id;
    }
    listener.close();
}

void Server::makeHandshake(sf::TcpSocket &socket)
{
    std::string str;
    sf::Packet packet;
    if(socket.receive(packet) != sf::Socket::Done)
        throw std::runtime_error("Could not receive data from client");
    packet >> str;
    if(str != "REQUEST_JOIN")
    {
        packet.clear();
        packet << "PROTOCOL_ERROR";
        socket.send(packet);
        
        throw std::runtime_error("Protocol error from client");
    }
    
    packet.clear();
    packet << "JOIN" << m_id;
    if(socket.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Could not send data to client");
    packet.clear();
    
    unsigned int clientID;
    if(socket.receive(packet) != sf::Socket::Done)
        throw std::runtime_error("Could not receive data from client");
    packet >> str >> clientID;
    if(str != "ACCEPT_JOIN" || clientID != m_id)
    {
        packet.clear();
        packet << "PROTOCOL_ERROR";
        socket.send(packet);
        
        throw std::runtime_error("Protocol error from client");
    }
}

void Server::addClient(const unsigned int clientID)
{
    unsigned short port = static_cast<unsigned short>(Server::getControlPort() + clientID);
    sf::TcpListener listener;

    if(listener.listen(port) != sf::Socket::Done)
    {
        std::stringstream ss;
        ss << "Could not listen on port ";
        ss << port;
        throw std::runtime_error(ss.str());
    }
    std::unique_ptr<sf::TcpSocket> clientSocket(new sf::TcpSocket());
    if(listener.accept(*clientSocket) != sf::Socket::Done)
        throw std::runtime_error("Could not accept client");
    listener.close();
    {
        std::lock_guard<std::mutex> lock(m_clientMutex);
        m_clients[clientID] = std::move(clientSocket);
        m_selector.add(*m_clients[clientID]);
    }

}

void Server::runControl()
{
    while(m_control)
    {
        if(m_selector.wait(sf::milliseconds(20)))
        {
            std::lock_guard<std::mutex> lock(m_clientMutex);
            for(auto it = m_clients.begin(); it != m_clients.end(); ++it)
            {
                auto& c = it->second;
                if(m_selector.isReady(*c))
                {
                    sf::Packet packet;
                    if(c->receive(packet) == sf::Socket::Done) // TODO: handle error
                    {
                        std::string str;
                        unsigned int clientID;
                        packet >> str >> clientID;
                        if(m_clients.find(clientID) == m_clients.end())
                        {
                            Logger::warning() << "Received data from unknown client.\n";
                            continue;
                        }

                        if(str == "REQUEST_QUIT")
                        {
                            packet.clear();
                            packet << "QUIT" << clientID;
                            c->send(packet);

                            // TODO: Broadcast to all other clients         
                            
                            // remove client
                            c->disconnect();
                            m_selector.remove(*c);
                            m_clients.erase(it);
                            break;
                        }
                    }
                }
            }
        }
        if(m_sendWorld)
        {
            sf::Packet packet;
            packet << "INIT_WORLD";
            packet << static_cast<unsigned int>(m_world.size());
            for(auto& s : m_world)
                packet << s;
            {
                std::lock_guard<std::mutex> lock(m_clientMutex);
                for(auto& c : m_clients)
                {
                    std::unique_ptr<sf::TcpSocket> &socket = c.second;
                    socket->send(packet); // TODO: Check return value
                }
            }
            m_sendWorld = false;
            Logger::info() << "Sent world to clients.\n";
        }
    }

    {
        std::lock_guard<std::mutex> lock(m_clientMutex);
        for(auto& c : m_clients)
        {
            std::unique_ptr<sf::TcpSocket> &socket = c.second;   
            sf::Packet packet;
            packet << "SERVER_EXIT";
            socket->setBlocking(true);
            socket->send(packet);
            socket->disconnect();
        }
    }
}

void Server::runData()
{
    sf::IpAddress clientAddress;
    unsigned short clientPort;
    std::map<unsigned int, std::unique_ptr<sf::UdpSocket>> recvSockets;
    sf::SocketSelector s;
    for(auto &c : m_clients)
    {
        unsigned int clientID = c.first;
        std::unique_ptr<sf::UdpSocket> ptr(new sf::UdpSocket());
        unsigned short port = static_cast<unsigned short>(Server::getDataPort() - clientID);
        ptr->bind(port); // TODO: Check return value
        recvSockets[clientID] = std::move(ptr);
        s.add(*recvSockets[clientID]);
    }
    std::map<unsigned int, std::unique_ptr<sf::UdpSocket>> sendSockets;
    for(auto &c : m_clients)
    {
        std::unique_ptr<sf::UdpSocket> ptr(new sf::UdpSocket());
        ptr->setBlocking(false);
        sendSockets[c.first] = std::move(ptr);
    }
    while(m_running)
    {
        if(s.wait())
        {
            for(auto &r : recvSockets)
            {
                std::unique_ptr<sf::UdpSocket> &socket = r.second;
                if(s.isReady(*socket))
                {
                    sf::Packet packet;
                    socket->receive(packet, clientAddress, clientPort);

                    NetworkEvent e;
                    e.fromPacket(packet);
                    packet.clear();

                    // Change stepNumber to handle latency
                    e.stepID += 5;  // TODO: change number depending on clients' latency
                    e.toPacket(packet);

                    // Broadcast event to all other clients
                    {
                        std::lock_guard<std::mutex> lock(m_clientMutex);
                        for(auto &c : m_clients)
                        {
                            sf::IpAddress address = c.second->getRemoteAddress();
                            unsigned int id = c.first;
                            unsigned short port = static_cast<unsigned short>(Server::getDataPort() + id);
                            sendSockets[id]->send(packet, address, port);
                        }
                    }
                }
            }
        }
    }
    for(auto &c : recvSockets)
        c.second->unbind();
}

unsigned short Server::getInitPort()
{
    return 49999;
}

unsigned short Server::getControlPort()
{
    return 19999;
}

unsigned short Server::getDataPort()
{
    return 29999;
}

