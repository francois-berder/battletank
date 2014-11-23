#ifndef __VIEW_H__
#define __VIEW_H__

#include <SFML/Graphics.hpp>
#include <queue>

#include "Tree.hpp"
#include "Event.hpp"

class Game;

class View
{
	public:

        View();

        void init(sf::WindowHandle handle);
        void update(Array &entities);

        static float gfxToGame(float);
        static float gameToGfx(float);
        
        void disableUserInput();
        
        bool pollEvent(Event &evt);

	private:

		void proceedEvents();
        void draw(Array &entities);
		void drawBackground();

		sf::RenderWindow m_window;
		bool m_disableUserInput;
        std::queue<Event> m_events;
        bool m_canShoot;
};

#endif /* __VIEW_H__ */

