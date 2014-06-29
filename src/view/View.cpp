#include "View.hpp"
#include "Game.hpp"


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
    
    m_window.display();
}
