#include "EntityViewer.hpp"
#include "TextureManager.hpp"
#include "View.hpp"
#include "Utils.hpp"


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
            x = View::gameToGfx(x);
            y = View::gameToGfx(y);
            drawObstacle(x, y);
        }
        else if(type == "tank")
        {
            Map &pos = entity["pos"]->asMap();
            float x = toFloat(pos["x"]->asData().getValue());
            float y = toFloat(pos["y"]->asData().getValue());
            x = View::gameToGfx(x);
            y = View::gameToGfx(y);
            int angle = toInteger(entity["angle"]->asData().getValue());
            int cannonAngle = toInteger(entity["cannonAngle"]->asData().getValue());
            unsigned int health = toUInteger(entity["health"]->asData().getValue());
            drawTank(x, y, angle, cannonAngle, health);
        }
        else if(type == "bullet")
        {
            Map &pos = entity["pos"]->asMap();
            float x = toFloat(pos["x"]->asData().getValue());
            float y = toFloat(pos["y"]->asData().getValue());
            x = View::gameToGfx(x);
            y = View::gameToGfx(y);
            int angle = toInteger(entity["angle"]->asData().getValue());
            drawBullet(x, y, angle);
        }
        else if(type == "explosion")
        {
            Map &pos = entity["pos"]->asMap();
            float x = toFloat(pos["x"]->asData().getValue());
            float y = toFloat(pos["y"]->asData().getValue());
            x = View::gameToGfx(x);
            y = View::gameToGfx(y);
            unsigned int timer = toUInteger(entity["timer"]->asData().getValue());
            drawExplosion(x, y, timer);
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

void EntityViewer::drawTank(float x, float y, int angle, int cannonAngle, unsigned int health)
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

    if(health == 0)
        lowerPartSprite.setColor(sf::Color(100, 100, 100));

    m_renderWindow.draw(lowerPartSprite);
    
    // Draw upper part
    TexturePtr upperPartTex = TextureManager::instance().get("tank_upper_part.png");
    sf::Sprite upperPartSprite(*upperPartTex);
    upperPartSprite.setPosition(x, y);
    a = static_cast<float>(cannonAngle);
    a += 5.6f;
    while(a < 0.f)
        a += 360.f;
    while(a >= 360.f)
        a -= 360.f;
    rectY = static_cast<unsigned int>(a / 56.25f);
    rectX = static_cast<unsigned int>((a - static_cast<float>(rectY) * 56.25f) / 11.25);
    upperPartSprite.setTextureRect(sf::IntRect(rectX * 160, rectY * 160, 160, 160));
    upperPartSprite.setOrigin(80, 80);

    if(health == 0)
        upperPartSprite.setColor(sf::Color(100, 100, 100));

    m_renderWindow.draw(upperPartSprite);
    
    // Draw upper part shadow
    TexturePtr upperPartShadowTex = TextureManager::instance().get("tank_upper_part_shadow.png");
    sf::Sprite upperPartShadowSprite(*upperPartShadowTex);
    upperPartShadowSprite.setPosition(x, y);
    upperPartShadowSprite.setTextureRect(sf::IntRect(rectX * 160, rectY * 160, 160, 160));
    upperPartShadowSprite.setOrigin(80, 80);
    m_renderWindow.draw(upperPartShadowSprite);

    // Draw health bar
	sf::RectangleShape greenBar;
	greenBar.setFillColor(sf::Color::Green);
	greenBar.setSize(sf::Vector2f(50, 4));
	greenBar.setOrigin(sf::Vector2f(25, 2));
	greenBar.setPosition(x, y - 60);
	m_renderWindow.draw(greenBar);

	sf::RectangleShape redBar;
	redBar.setFillColor(sf::Color::Red);
	float sizeRedBarX = (100.f - static_cast<float>(health)) / 2.f;
	redBar.setSize(sf::Vector2f(sizeRedBarX, 4));
	redBar.setOrigin(sf::Vector2f(sizeRedBarX, 2));
	redBar.setPosition(x + 25, y - 60);
	m_renderWindow.draw(redBar);
}

void EntityViewer::drawBullet(float x, float y, int angle)
{
    TexturePtr tex = TextureManager::instance().get("bullet.png");
    sf::Sprite bullet(*tex);
    bullet.setOrigin(6, 2);
    bullet.setPosition(x, y);
    bullet.rotate(static_cast<float>(angle));
    m_renderWindow.draw(bullet);
}

void EntityViewer::drawExplosion(float x, float y, unsigned int timer)
{
    TexturePtr tex = TextureManager::instance().get("tank_explosion.png");
    sf::Sprite explosion(*tex);
    timer /= 4;
    unsigned int rectY = 3 - timer / 4;
    unsigned int rectX = 3 - timer % 4;
    explosion.setTextureRect(sf::IntRect(rectX * 64, rectY * 64, 64, 64));
    explosion.setOrigin(32, 32);
    explosion.setPosition(x, y);
    m_renderWindow.draw(explosion);
}

