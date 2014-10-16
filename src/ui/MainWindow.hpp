#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>

#include "MainMenu.hpp"
#include "PlayMenu.hpp"
#include "ChatMenu.hpp"

class MainWindow : public QStackedWidget
{
    Q_OBJECT

    public :

        MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();

    private slots :

        void createServer(QString);
        void createClient(QString, QString);

    private :

        MainMenu *m_mainMenu;
        PlayMenu *m_playMenu;
        ChatMenu *m_chatMenu;
};

#endif // MAINWINDOW_H
