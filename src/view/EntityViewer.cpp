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
            int angle = toInteger(entity["angle"]->asData().getValue());
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

void EntityViewer::drawTank(float x, float y, int angle, unsigned int health)
{
    // Draw lower part
    TexturePtr lowerPartTex = TextureManager::instance().get("tank_lower_part.png");
    sf::Sprite lowerPartSprite(*lowerPartTex);
    lowerPartSprite.setPosition(x, y);
    float a = static_cast<float>(angle);
    a += 5.6f;
    while(a < 0.f)
        a += 360.f;
    while(a >= 360.f)
        a -= 360.f;
    unsigned int rectY = static_cast<unsigned int>(a / 56.25f);
    unsigned int rectX = static_cast<unsigned int>((a - static_cast<float>(rectY) * 56.25f) / 11.25);
    lowerPartSprite.setTextureRect(sf::IntRect(rectX * 160, rectY * 160, 160, 160));
    lowerPartSprite.setOrigin(80, 80);

    m_renderWindow.draw(lowerPartSprite);
    
    // Draw upper part
    TexturePtr upperPartTex = TextureManager::instance().get("tank_upper_part.png");
    sf::Sprite upperPartSprite(*upperPartTex);
    upperPartSprite.setPosition(x, y);
    upperPartSprite.setTextureRect(sf::IntRect(rectX * 160, rectY * 160, 160, 160));
    upperPartSprite.setOrigin(80, 80);
    m_renderWindow.draw(upperPartSprite);
    
    // Draw upper part shadow
    TexturePtr upperPartShadowTex = TextureManager::instance().get("tank_upper_part_shadow.png");
    sf::Sprite upperPartShadowSprite(*upperPartShadowTex);
    upperPartShadowSprite.setPosition(x, y);
    upperPartShadowSprite.setTextureRect(sf::IntRect(rectX * 160, rectY * 160, 160, 160));
    upperPartShadowSprite.setOrigin(80, 80);
    m_renderWindow.draw(upperPartShadowSprite);

    // TODO: Draw health bar
}

