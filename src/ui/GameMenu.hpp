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

    public slots :

        void host();
        void join(QString serverAddress, QString pseudo);
        void start();

    private :
        virtual void keyPressEvent(QKeyEvent * event);

        Game m_game;
        QTimer m_timer;
        bool m_initialized;
};

#endif // GAMEMENU_HPP
