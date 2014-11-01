#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <thread>
#include <mutex>
#include <memory>
#include <list>
#include <SFML/Network.hpp>

class Server
{
    public :
    
        Server();
        ~Server();
        
        void startAcceptingClients();
        void stopAcceptingClients();

        void start();
        void stop();
        
        void initWorld();

        static unsigned short getInitPort();
        static unsigned short getControlPort();
        static unsigned short getDataPort();

        bool isRunning() const;
        bool hasClients() const;

    private :
    
        void runInit();
        void makeHandshake(sf::TcpSocket &socket, std::string &clientName);
        void addClient(const unsigned int clientID, const std::string& clientName);
        
        void runControl();
        
        void runData();
        
        std::thread m_initThread;
        std::thread m_controlThread;
        std::thread m_dataThread;
        
        bool m_acceptClients;
        bool m_control;
        bool m_data;
        bool m_running;
        unsigned int m_id;
        std::map<unsigned int, std::unique_ptr<sf::TcpSocket>> m_clients;
        std::map<std::string, unsigned int> m_clientNames;
        sf::SocketSelector m_selector;
        std::mutex m_clientMutex;
        bool m_sendWorld;
        std::list<std::string> m_world;
};

#endif /* __SERVER_HPP__ */

