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
m_data(false),
m_running(false),
m_id(1),
m_clients(),
m_clientNames(),
m_initListener(),
m_selector(),
m_clientMutex(),
m_world()
{
}

Server::~Server()
{
    stop();
}

void Server::startAcceptingClients(std::list<std::string> clientNames)
{
    for(auto& name : clientNames)
        m_clientNames[name] = 0;

    // reset id counter
    m_id = 1;

    // create world
    createWorld();

    m_initListener.close();
    unsigned short port = Server::getInitPort();
    if(m_initListener.listen(port) != sf::Socket::Done)
    {
        std::stringstream ss;
        ss << "Could not listen on port " << port;
        throw std::runtime_error(ss.str());
    }
    Logger::info() << "Listening for incoming players on port " << port << ".\n";

    m_acceptClients = true;
    std::thread t(&Server::runInit, this);
    m_initThread.swap(t);
}

void Server::runInit()
{
    sf::SocketSelector selector;
    selector.add(m_initListener);

    while(m_acceptClients)
    {
        if(selector.wait(sf::milliseconds(20)))
        {
            if(selector.isReady(m_initListener))
            {
                sf::TcpSocket initClientSocket;
                sf::Socket::Status ret = m_initListener.accept(initClientSocket);
                if(ret != sf::Socket::Done)
                    continue;

                std::string clientName;
                try
                {
                    makeHandshake(initClientSocket, clientName);
                }
                catch(std::exception &e)
                {
                    Logger::error() << "Handshake failed with client. " << e.what() << '\n';
                }
                addClient(m_id, clientName); // TODO: Handle exceptions

                sendWorld(m_id);

                Logger::info() << "New player: " << clientName << " (id=" << m_id << ") from " << initClientSocket.getRemoteAddress().toString() << '\n';
                initClientSocket.disconnect();
                ++m_id;
            }
        }
    }
}

// clientNames: list of pseudo of all clients that should join this game.
// timout in seconds
void Server::waitUntilAllClientsConnected(const float timeout)
{
    sf::Clock clock;

    /* Wait for incoming clients until:
     * _ timeout
     * or
     * _ all clients are connected
     */
    while(clock.getElapsedTime().asSeconds() < timeout && m_clientNames.size() != m_clients.size())
    {
        sf::sleep(sf::milliseconds(50));
    }

    m_acceptClients = false;
    m_initThread.join();

    m_initListener.close();
    if(m_clientNames.size() != m_clients.size())
        throw std::runtime_error("Timeout. Not all clients are connected");
}

void Server::waitUntilAllClientsDisconnected()
{
    if(!m_running)
        return;

    while(!m_clients.empty())
        sf::sleep(sf::milliseconds(100));
}

void Server::start()
{
    if(m_running)
        return;

    m_running = true;
    m_data = true;
    m_control = true;
    std::thread data(&Server::runData, this);
    m_dataThread.swap(data);
    std::thread control(&Server::runControl, this);
    m_controlThread.swap(control);

    Logger::info() << "Server started.\n";
}

void Server::stop()
{
    if(!m_running)
        return;

    m_data = false;
    m_running = false;
    m_control = false;
    m_controlThread.join();
    m_dataThread.join();
    m_clients.clear();
    m_selector.clear();

    Logger::info() << "Server stopped.\n";
}

// Precompute world from list of clients
void Server::createWorld()
{
    m_world.clear();
    int i = 0;
    for(auto& c : m_clientNames)
    {
        std::stringstream ss;
        ss << "0 new tank ";
        ss << c.first;
        ss << " ";
        ss << i * 3;
        ss << " ";
        ss << 5;
        m_world.push_back(ss.str());
        ++i;
    }

    m_world.push_back("0 new obstacle 10 11");
    m_world.push_back("0 new obstacle 10 10");
}

void Server::makeHandshake(sf::TcpSocket &socket, std::string &clientName)
{
    std::string str;
    sf::Packet packet;
    if(socket.receive(packet) != sf::Socket::Done)
        throw std::runtime_error("Could not receive data from client");
    packet >> str >> clientName;
    if(str != "REQUEST_JOIN")
    {
        packet.clear();
        packet << "PROTOCOL_ERROR";
        socket.send(packet);
        
        throw std::runtime_error("Protocol error from client");
    }
    // TODO: check that clientName belongs to m_clientNames
    packet.clear();
    packet << "JOIN" << clientName << m_id;
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

//TODO: to change this
void Server::addClient(const unsigned int clientID, const std::string& clientName)
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
        m_clientNames[clientName] = clientID;
        m_selector.add(*m_clients[clientID]);
    }
}

void Server::sendWorld(const unsigned int clientID)
{
    sf::Packet packet;
    packet << "INIT_WORLD";
    packet << static_cast<unsigned int>(m_world.size());
    for(auto& s : m_world)
        packet << s;
    m_clients[clientID]->send(packet);
}

void Server::runControl()
{
    while(m_control)
    {
        if(m_selector.wait(sf::milliseconds(50)))
        {
            std::lock_guard<std::mutex> lock(m_clientMutex);
            std::list<unsigned int> clientsToRemove;
            auto it = m_clients.begin();
            while(it != m_clients.end())
            {
                auto& c = it->second;
                if(m_selector.isReady(*c))
                {
                    sf::Packet packet;
                    c->receive(packet);
                    std::string str;
                    unsigned int clientID;
                    packet >> str >> clientID;
                    if(m_clients.find(clientID) == m_clients.end())
                    {
                        Logger::warning() << "Received data from unknown client.\n";
                        ++it;
                        continue;
                    }

                    if(str == "REQUEST_QUIT")
                    {
                        packet.clear();
                        packet << "QUIT" << clientID;
                        c->send(packet);

                        // remove client later
                        clientsToRemove.push_back(clientID);
                    }
                }
                ++it;
            }
            for(auto clientID : clientsToRemove)
            {
                m_selector.remove(*m_clients[clientID]);
                m_clients.erase(clientID);
            }
        }
    }

    {
        std::lock_guard<std::mutex> lock(m_clientMutex);
        for(auto& c : m_clients)
        {
            std::unique_ptr<sf::TcpSocket> &socket = c.second;   
            sf::Packet packet;
            packet << "SERVER_EXIT";
            socket->send(packet);
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
    while(m_data)
    {
        if(s.wait(sf::milliseconds(50)))
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
