#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>

#include "MainMenu.hpp"
#include "PlayMenu.hpp"

class MainWindow : public QStackedWidget
{
    Q_OBJECT

    public :

        MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();

    private :

        MainMenu *m_mainMenu;
        PlayMenu *m_playMenu;
};

#endif // MAINWINDOW_H
