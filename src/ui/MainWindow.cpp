#include "MainWindow.hpp"
#include "Menu.hpp"

MainWindow::MainWindow(QWidget *parent) :
QStackedWidget(parent),
m_mainMenu(new MainMenu(this)),
m_playMenu(new PlayMenu(this)),
m_chatMenu(new ChatMenu(this)),
m_gameMenu(new GameMenu(this))
{
    this->setFixedWidth(800);
    this->setFixedHeight(600);

    insertWidget(MAIN_MENU, m_mainMenu);
    insertWidget(PLAY_MENU, m_playMenu);
    insertWidget(CHAT_MENU, m_chatMenu);
    insertWidget(GAME_MENU, m_gameMenu);

    QObject::connect(m_mainMenu, SIGNAL(changeInterface(int)), this, SLOT(setCurrentIndex(int)));
    QObject::connect(m_playMenu, SIGNAL(changeInterface(int)), this, SLOT(setCurrentIndex(int)));
    QObject::connect(m_chatMenu, SIGNAL(changeInterface(int)), this, SLOT(setCurrentIndex(int)));
    QObject::connect(m_chatMenu, SIGNAL(gameStarted()), m_gameMenu, SLOT(start()));
    QObject::connect(m_chatMenu, SIGNAL(hostLaunchedGame()), m_gameMenu, SLOT(host()));
    QObject::connect(m_chatMenu, SIGNAL(playerReceivedGameLaunch(QString, QString)), m_gameMenu, SLOT(join(QString, QString)));
    QObject::connect(m_playMenu, SIGNAL(createServer(QString)), this, SLOT(createServer(QString)));
    QObject::connect(m_playMenu, SIGNAL(createClient(QString, QString)), this, SLOT(createClient(QString, QString)));
}

MainWindow::~MainWindow()
{
    delete m_mainMenu;
    delete m_playMenu;
    delete m_chatMenu;
}

void MainWindow::createServer(QString pseudo)
{
    m_chatMenu->host(pseudo);
    setCurrentIndex(CHAT_MENU);
}

void MainWindow::createClient(QString pseudo, QString serverAddress)
{
    m_chatMenu->join(pseudo, serverAddress);
    setCurrentIndex(CHAT_MENU);
}
