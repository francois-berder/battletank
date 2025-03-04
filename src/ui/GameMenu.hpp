#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

#include <QWidget>
#include <QTimer>
#include "Game.hpp"

class GameMenu : public QWidget
{
    Q_OBJECT

    public :

        explicit GameMenu(QWidget *parent = 0);
        virtual ~GameMenu();

        virtual void showEvent(QShowEvent*);
        virtual QPaintEngine* paintEngine() const;
        virtual void paintEvent(QPaintEvent*);

    signals :

        void changeInterface(int);

    public slots :

        void host(QList<QString> clients, QString pseudo);
        void join(QString serverAddress, QString pseudo);

    private :

        void startGame();

        Game m_game;
        QTimer m_timer;
        bool m_initialized;
};

#endif // GAMEMENU_HPP
