#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <thread>
#include <SFML/Network.hpp>
#include <QObject>
#include <QString>

class Player : public QObject
{
    Q_OBJECT

    public :

        Player();

        void join(QString serverAddress);
        void leave();
        void abortLaunch();

        bool isConnected();

        void sendMessage(QString message);
        void setReadiness(bool isReady);

        void setPseudo(QString pseudo);
        QString getPseudo() const;

        QString getIPAddress() const;
        QString getHostIPAddress() const;

    signals :

        void receivedMessage(QString, QString);
        void playerJoined(QString);
        void playerLeft(QString);
        void playerReady(QString);
        void playerNotReady(QString);
        void gameCancelled();
        void gameLaunchStarted();
        void gameLaunchAborted(QString);
        void existingText(QString);

    private :

        void run();
        void handleData(sf::Packet &packet);

        QString m_pseudo;
        bool m_joinedGame;
        bool m_gameCancelled;
        bool m_gameLaunchStarted;
        std::thread m_thread;
        sf::TcpSocket m_socket;
};

#endif // PLAYER_HPP
