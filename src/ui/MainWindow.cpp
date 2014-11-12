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

    QObject::connect(m_mainMenu, SIGNAL(changeInterface(int)), this, SLOT(setCurrentIndex(int)), Qt::QueuedConnection);

    QObject::connect(m_playMenu, SIGNAL(changeInterface(int)), this, SLOT(setCurrentIndex(int)), Qt::QueuedConnection);
    QObject::connect(m_playMenu, SIGNAL(createServer(QString)), this, SLOT(createChatServer(QString)));
    QObject::connect(m_playMenu, SIGNAL(createClient(QString, QString)), this, SLOT(createChatClient(QString, QString)));

    QObject::connect(m_chatMenu, SIGNAL(changeInterface(int)), this, SLOT(setCurrentIndex(int)), Qt::QueuedConnection);
    QObject::connect(m_chatMenu, SIGNAL(createServer(QList<QString>,QString)), this, SLOT(createGameServer(QList<QString>, QString)));
    QObject::connect(m_chatMenu, SIGNAL(createClient(QString, QString)), this, SLOT(createGameClient(QString, QString)));

    QObject::connect(m_gameMenu, SIGNAL(changeInterface(int)), this, SLOT(setCurrentIndex(int)), Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete m_mainMenu;
    delete m_playMenu;
    delete m_chatMenu;
    delete m_gameMenu;
}

void MainWindow::createChatServer(QString pseudo)
{
    m_chatMenu->host(pseudo);
    setCurrentIndex(CHAT_MENU);
}

void MainWindow::createChatClient(QString serverAddress, QString pseudo)
{
    m_chatMenu->join(serverAddress, pseudo);
    setCurrentIndex(CHAT_MENU);
}

void MainWindow::createGameServer(QList<QString> clientPseudos, QString pseudo)
{
    m_gameMenu->host(clientPseudos, pseudo);
    setCurrentIndex(GAME_MENU);
}

void MainWindow::createGameClient(QString serverAddress, QString pseudo)
{
    m_gameMenu->join(serverAddress, pseudo);
    setCurrentIndex(GAME_MENU);
}
