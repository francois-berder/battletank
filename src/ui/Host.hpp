#ifndef HOST_HPP
#define HOST_HPP

#include <QMap>
#include <QPair>
#include <QList>
#include <QString>
#include <thread>
#include <SFML/Network.hpp>
#include "GameData.hpp"


class Host
{

    public :

        Host(GameData &data);

        void start();
        void stop();

    private :

        void run();
        void handleIncomingClient();
        void handleData(QString pseudo, sf::TcpSocket &socket);

        std::thread m_thread;
        sf::TcpListener m_listener;
        sf::SocketSelector m_selector;
        QMap<QString, sf::TcpSocket*> m_sockets;
        QList<QString> m_socketsToRemove;
        GameData &m_data;
        bool m_running;
};

#endif // HOST_HPP
