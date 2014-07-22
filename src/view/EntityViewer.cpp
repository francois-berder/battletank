#include "EntityViewer.hpp"
#include "TextureManager.hpp"


EntityViewer::EntityViewer(sf::RenderWindow &renderWindow, Array &entities):
m_renderWindow(renderWindow),
m_entities(entities)
{
}

void EntityViewer::drawAll()
{
    // TODO: Implement function
}

void EntityViewer::drawObstacle(float x, float y)
{
    TexturePtr tex = TextureManager::instance().get("obstacle.png");
    sf::Sprite sprite(*tex);
    sprite.setPosition(x, y);
    
    m_renderWindow.draw(sprite);
}

void EntityViewer::drawTank(float x, float y, float angle, unsigned int health)
{
    // TODO: Implement function
}

