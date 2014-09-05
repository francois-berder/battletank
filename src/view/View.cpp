#include "View.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include "Event.hpp"
#include "EntityViewer.hpp"
#include "EntityParser.hpp"


View::View():
m_window(sf::VideoMode(800, 600), "Battle Tank"), 
m_disableUserInput(false),
m_events()
{
}

void View::update(const std::string& gameState)
{
	proceedEvents();
	draw(gameState);
}

bool View::pollEvent(Event &evt)
{
    if(m_events.empty())
        return false;

    evt = m_events.front();
    m_events.pop();

    return true;
}

void View::proceedEvents()
{
    if(!m_disableUserInput)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		    m_events.push(EventType::Left);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		    m_events.push(EventType::Right);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		    m_events.push(EventType::Down);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		    m_events.push(EventType::Up);
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		m_events.push(EventType::Quit);

	sf::Event e;
	while(m_window.pollEvent(e))
	{
		if(e.type == sf::Event::Closed)
			m_events.push(EventType::Quit);
			
		if(e.type == sf::Event::MouseMoved && !m_disableUserInput)
		{
		    float x = static_cast<float>(e.mouseMove.x);
		    float y = static_cast<float>(e.mouseMove.y);
		    x = gfxToGame(x);
		    y = gfxToGame(y);
		    m_events.push(Event(x, y));
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

void View::disableUserInput()
{
    m_disableUserInput = true;
}
