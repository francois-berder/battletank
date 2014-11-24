#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>

#include "MainMenu.hpp"
#include "PlayMenu.hpp"
#include "ChatMenu.hpp"
#include "GameMenu.hpp"
#include "StatMenu.hpp"

class MainWindow : public QStackedWidget
{
    Q_OBJECT

    public :

        MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();

    private slots :

        void createChatServer(QString pseudo);
        void createChatClient(QString serverAddress, QString pseudo);
        void createGameServer(QList<QString> clientPseudos, QString pseudo);
        void createGameClient(QString serverAddress, QString pseudo);

    private :

        MainMenu *m_mainMenu;
        PlayMenu *m_playMenu;
        ChatMenu *m_chatMenu;
        GameMenu *m_gameMenu;
        StatMenu *m_statMenu;
};

#endif // MAINWINDOW_H
