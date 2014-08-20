#include "View.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include "Event.hpp"
#include "EntityViewer.hpp"
#include "EntityParser.hpp"


View::View(Game &game) :
		m_game(game), m_window(sf::VideoMode(800, 600), "Battle Tank")
{
}

void View::update(const std::string& gameState)
{
	proceedEvents();
	draw(gameState);
}

void View::proceedEvents()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_game.pushEvent(EventType::Left);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_game.pushEvent(EventType::Right);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_game.pushEvent(EventType::Down);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_game.pushEvent(EventType::Up);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		m_game.pushEvent(EventType::Quit);

	sf::Event e;
	while(m_window.pollEvent(e))
	{
		if(e.type == sf::Event::Closed)
			m_game.pushEvent(EventType::Quit);
			
		if(e.type == sf::Event::MouseMoved)
		{
		    float x = static_cast<float>(e.mouseMove.x);
		    float y = static_cast<float>(e.mouseMove.y);
		    x = gfxToGame(x);
		    y = gfxToGame(y);
		    m_game.pushEvent(Event(x, y));
	    }
	}
}

float View::gfxToGame(float a)
{
    return a / 40.f;
}

float View::gameToGfx(float a)
{
    return a * 40.f;
}
        
void View::draw(const std::string &gameState)
{
	m_window.clear();
	drawBackground();
	drawState(gameState);
	m_window.display();
}

void View::drawBackground()
{
	TexturePtr tex = TextureManager::instance().get("background.png");
	sf::Sprite background(*tex);
	m_window.draw(background);
}

void View::drawState(const std::string &state)
{
    EntityParser parser(state);
    Array entities = parser.parse();
    EntityViewer v(m_window, entities);
    v.drawAll();
}

