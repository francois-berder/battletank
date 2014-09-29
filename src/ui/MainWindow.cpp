#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
QStackedWidget(parent),
m_mainMenu(new MainMenu)
{
    insertWidget(MAIN_MENU, m_mainMenu);
}

MainWindow::~MainWindow()
{
    delete m_mainMenu;
}
