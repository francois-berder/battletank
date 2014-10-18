#include <QTimer>
#include <list>
#include <string>
#include "GameMenu.hpp"


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

    std::list<std::string> args;
    args.push_back("--disable-network-client");
    std::list<Option> options = Option::parse(args);
    m_game.setOptions(options);
}

GameMenu::~GameMenu()
{

}

void GameMenu::showEvent(QShowEvent*)
{
    if (!m_initialized)
    {
        m_game.init(winId());

        // Setup the timer to trigger a refresh at specified framerate
        connect(&m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
        m_timer.start();
        m_initialized = true;
    }
}

QPaintEngine* GameMenu::paintEngine() const
{
    return 0;
}

void GameMenu::paintEvent(QPaintEvent*)
{
    m_game.update();
}
