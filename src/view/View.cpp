#include "View.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include "Event.hpp"


View::View(Game &game):
m_game(game),
m_window(sf::VideoMode(800, 600), "Battle Tank")
{
}

void View::update(const std::string& gameState)
{
    proceedEvents();
    
    draw();
}

void View::proceedEvents()
{
    sf::Event e;

    while(m_window.pollEvent(e))
    {
        if(e.type == sf::Event::Closed)
            m_game.pushEvent(QuitEvent);
        if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
            m_game.pushEvent(QuitEvent);
    }
}

void View::draw()
{
    m_window.clear();
    drawBackground();
    m_window.display();
}

void View::drawBackground()
{
    TexturePtr tex = TextureManager::instance().get("background.png");
    sf::Sprite background(*tex);
    m_window.draw(background);
}
