#include "EntityViewer.hpp"
#include "TextureManager.hpp"
#include "Utils.hpp"


namespace
{
    const float GAME_TO_GFX_COORD = 40.f;
}

EntityViewer::EntityViewer(sf::RenderWindow &renderWindow, Array &entities):
m_renderWindow(renderWindow),
m_entities(entities)
{
}

void EntityViewer::drawAll()
{
    for(unsigned int i = 0; i < m_entities.size(); ++i)
    {
        Map &entity = m_entities[i]->asMap();
        std::string type = entity["type"]->asData().getValue();
        
        if(type == "obstacle")
        {
            Map &pos = entity["pos"]->asMap();
            float x = toFloat(pos["x"]->asData().getValue());
            float y = toFloat(pos["y"]->asData().getValue());
            x *= GAME_TO_GFX_COORD;
            y *= GAME_TO_GFX_COORD;
            drawObstacle(x, y);
        }
        else if(type == "tank")
        {
            Map &pos = entity["pos"]->asMap();
            float x = toFloat(pos["x"]->asData().getValue());
            float y = toFloat(pos["y"]->asData().getValue());
            x *= GAME_TO_GFX_COORD;
            y *= GAME_TO_GFX_COORD;
            float angle = toFloat(entity["angle"]->asData().getValue());
            unsigned int health = toUInteger(entity["health"]->asData().getValue());
            drawTank(x, y, angle, health);
        }
    }
}

void EntityViewer::drawObstacle(float x, float y)
{
    TexturePtr tex = TextureManager::instance().get("obstacle.png");
    sf::Sprite sprite(*tex);
    sprite.setPosition(x, y);
    float oX = static_cast<float>(tex->getSize().x) / 2.f;
    float oY = static_cast<float>(tex->getSize().y) / 2.f;
    sprite.setOrigin(oX, oY);
    m_renderWindow.draw(sprite);
}

void EntityViewer::drawTank(float x, float y, float angle, unsigned int health)
{
    // Draw lower part
    TexturePtr tex = TextureManager::instance().get("tank_lower_part.png");
    sf::Sprite lowerPartSprite(*tex);
    lowerPartSprite.setPosition(x, y);
    angle = convertGradToDeg(angle);
    angle += 5.6f;
    if(angle < 0.f)
        angle += 360.f;
    if(angle >= 360.f)
        angle -= 360.f;
    unsigned int rectY = static_cast<unsigned int>(angle / 56.25f);
    unsigned int rectX = static_cast<unsigned int>((angle - static_cast<float>(rectY) * 56.25f) / 11.25);
    lowerPartSprite.setTextureRect(sf::IntRect(rectX * 160, rectY * 160, 160, 160));
    lowerPartSprite.setOrigin(80, 80);

    m_renderWindow.draw(lowerPartSprite);
    
    // TODO: Draw upper part
    // TODO: Draw health bar
}

