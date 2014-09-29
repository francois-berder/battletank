#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>
#include "MainMenu.hpp"

enum WINDOW_STATE
{
    MAIN_MENU
};

class MainWindow : public QStackedWidget
{
    Q_OBJECT

    public:

        MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();

    private :

        MainMenu *m_mainMenu;
};

#endif // MAINWINDOW_H
