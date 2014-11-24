#include <QTimer>
#include <QThread>
#include <QMessageBox>
#include <list>
#include <string>
#include "GameMenu.hpp"
#include "Menu.hpp"

GameMenu::GameMenu(QWidget *parent):
QWidget(parent),
m_game(),
m_timer(),
m_initialized(false)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);

    // Achieve 60 FPS
    m_timer.setInterval(17);
}

GameMenu::~GameMenu()
{

}

void GameMenu::showEvent(QShowEvent*)
{
    if (!m_initialized)
    {
        m_initialized = true;

        m_game.initView(winId());

        // Setup the timer to trigger a refresh at specified framerate
        connect(&m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
    }
}

QPaintEngine* GameMenu::paintEngine() const
{
    return 0;
}

void GameMenu::paintEvent(QPaintEvent*)
{
    if(!m_game.update())
    {
        m_game.exit();
        changeInterface(CHAT_MENU); // TODO: change to STATS_MENU instead
    }
}

void GameMenu::host(QList<QString> clients, QString pseudo)
{
    Server &server = m_game.getServer();
    Client &client  = m_game.getClient();

    try
    {
        // Convert Qt list of string to std list of std string
        std::list<std::string> clientNames;
        for(QList<QString>::iterator itor = clients.begin();
            itor != clients.end();
            ++itor)
            clientNames.push_back(itor->toStdString());

        server.startAcceptingClients(clientNames);
        client.connect("localhost", pseudo.toStdString());
        server.waitUntilAllClientsConnected(5.f);

        server.start();

        startGame();
    }
    catch(std::exception &e)
    {
        QMessageBox::critical(this, "Error while starting game", QString::fromStdString(e.what()));
        m_timer.stop();
        emit changeInterface(CHAT_MENU);
    }
}

void GameMenu::join(QString serverAddress, QString pseudo)
{
    sf::sleep(sf::milliseconds(500));
    m_game.getClient().connect(serverAddress.toStdString(), pseudo.toStdString());
    startGame();
}

void GameMenu::startGame()
{
    m_game.initWorld();
    m_timer.start();
}
