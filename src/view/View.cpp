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
		m_game.pushEvent(Event::Left);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_game.pushEvent(Event::Right);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_game.pushEvent(Event::Down);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_game.pushEvent(Event::Up);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		m_game.pushEvent(Event::Quit);

	sf::Event e;
	while(m_window.pollEvent(e))
	{
		if(e.type == sf::Event::Closed)
			m_game.pushEvent(Event::Quit);
	}
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


