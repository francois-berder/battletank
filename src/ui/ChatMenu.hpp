#ifndef CHATMENU_HPP
#define CHATMENU_HPP

#include <QWidget>
#include "Host.hpp"
#include "Player.hpp"
#include "GameData.hpp"

namespace Ui
{
    class ChatMenu;
}

class ChatMenu : public QWidget
{
    Q_OBJECT

    public :

        explicit ChatMenu(QWidget *parent = 0);
        virtual ~ChatMenu();

        void host(QString pseudo);
        void join(QString serverAddress, QString pseudo);

    signals :

        void changeInterface(int);
        void createServer(QList<QString>, QString);
        void createClient(QString, QString);

    private slots :

        void printMessage(QString pseudo, QString message);
        void addPlayer(QString pseudo);
        void removePlayer(QString pseudo);
        void sendMessage();
        void leave();
        void cancel();
        void sendPlayerReadiness(int);
        void playerReady(QString pseudo);
        void playerNotReady(QString pseudo);
        void reportGameLaunchStarted();
        void reportGameLaunchAborted(QString pseudo);

    private :

        void clean();
        void changePlayerReady(QString pseudo, bool isReady);
        void checkCanStart();

        Ui::ChatMenu *ui;
        Player m_player;
        Host m_host;
        GameData m_data;
};

#endif // CHATMENU_HPP
