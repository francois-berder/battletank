#include "View.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"


View::View(Game &game):
m_game(game),
m_window(sf::VideoMode(800, 600), "Battle Tank")
{
}

void View::update(const std::string& gameState)
{
    draw();
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
