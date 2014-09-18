#ifndef __VIEW_H__
#define __VIEW_H__

#include <SFML/Graphics.hpp>
#include <queue>

#include "Event.hpp"

class Game;

class View
{
	public:

		View();

		void update(const std::string& gameState);

        static float gfxToGame(float);
        static float gameToGfx(float);
        
        void disableUserInput();
        
        bool pollEvent(Event &evt);

	private:

		void proceedEvents();
		void draw(const std::string &state);
		void drawBackground();
		void drawState(const std::string &state);

		sf::RenderWindow m_window;
		bool m_disableUserInput;
        std::queue<Event> m_events;
        bool m_canShoot;
};

#endif /* __VIEW_H__ */

