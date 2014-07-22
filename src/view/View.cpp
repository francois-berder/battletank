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
	sf::Event e;

	while (m_window.pollEvent(e))
	{
		if(e.type == sf::Event::Closed)
			m_game.pushEvent(Event::Quit);
		if(e.type == sf::Event::KeyPressed
				&& e.key.code == sf::Keyboard::Escape)
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
    Array entities = EntityParser::parse(state);
    EntityViewer v(m_window, entities);
    v.drawAll();
}


