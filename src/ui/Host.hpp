#ifndef HOST_HPP
#define HOST_HPP

#include <QObject>
#include <QMap>
#include <QPair>
#include <QList>
#include <QString>
#include <thread>
#include <SFML/Network.hpp>
#include "GameData.hpp"


class Host : public QObject
{

    Q_OBJECT

    public :

        Host(GameData &data);

        void start();
        void stop();

        bool isRunning() const;
        QList<QString> getPlayersNames() const;

    signals :

        void gameLaunched();

    public slots :

        void launchGame();

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
        bool m_gameLaunched;
};

#endif // HOST_HPP
