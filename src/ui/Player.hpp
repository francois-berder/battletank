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

        void sendMessage(QString message);

        void setPseudo(QString pseudo);
        QString getPseudo() const;

    signals :

        void receivedMessage(QString, QString);
        void playerJoined(QString);
        void playerLeft(QString);

    private :

        void run();
        void handleData(sf::Packet &packet);

        QString m_pseudo;
        bool m_joinedGame;
        std::thread m_thread;
        sf::SocketSelector m_selector;
        sf::TcpSocket m_socket;
};

#endif // PLAYER_HPP
