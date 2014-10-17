#ifndef HOST_HPP
#define HOST_HPP

#include <QObject>
#include <QMap>
#include <QPair>
#include <QList>
#include <QString>
#include <thread>
#include <SFML/Network.hpp>

class Host
{
    public :

        Host();

        void start();
        void stop();

    private :

        void run();
        void handleIncomingClient();
        void handleData(QString pseudo, sf::TcpSocket &socket);

        std::thread m_thread;
        sf::TcpListener m_listener;
        sf::SocketSelector m_selector;
        QMap<QString, QPair<sf::TcpSocket*, bool> > m_players;
        QList<QString> m_playersToRemove;
        bool m_running;
};

#endif // HOST_HPP
