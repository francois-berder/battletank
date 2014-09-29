#include "MainWindow.hpp"
#include "Menu.hpp"

MainWindow::MainWindow(QWidget *parent) :
QStackedWidget(parent),
m_mainMenu(new MainMenu(this)),
m_playMenu(new PlayMenu(this))
{
    insertWidget(MAIN_MENU, m_mainMenu);
    insertWidget(PLAY_MENU, m_playMenu);

    QObject::connect(m_mainMenu, SIGNAL(changeInterface(int)), this, SLOT(setCurrentIndex(int)));
    QObject::connect(m_playMenu, SIGNAL(changeInterface(int)), this, SLOT(setCurrentIndex(int)));
}

MainWindow::~MainWindow()
{
    delete m_mainMenu;
    delete m_playMenu;
}
