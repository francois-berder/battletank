#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <thread>
#include <string>
#include <SFML/Network.hpp>
#include <queue>
#include <mutex>
#include <list>

#include "NetworkEvent.hpp"

class Client
{
    public :
    
        Client();
        ~Client();
        
        unsigned int connect(const std::string ipAddress);
        void disconnect();
        
        bool pollEvent(NetworkEvent &evt);
        void pushEvent(NetworkEvent &evt);

        unsigned int getID() const;
        std::list<std::string> getWorld();

    private :
        
        void runControl();
        void makeHandshake(const std::string &ipAddress);
        void sendExitSequence();
        void runData();        
        Event convertPacketToEvent(sf::Packet &packet);

        sf::TcpSocket m_controlSocket;
        std::thread m_controlThread;
        sf::UdpSocket m_dataSocket;
        std::thread m_dataThread;
        bool m_isConnected;
        unsigned int m_id;
        std::queue<NetworkEvent> m_events;
        std::mutex m_eventsMutex;
        std::queue<NetworkEvent> m_toSendEvents;
        std::mutex m_toSendEventsMutex;
        std::list<std::string> m_initCommands;
        std::mutex m_initCommandsMutex;
};

#endif /* __CLIENT_H__ */

