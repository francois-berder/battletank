#ifndef __VIEW_H__
#define __VIEW_H__

#include <SFML/Graphics.hpp>

class Game;

class View
{
	public:

		View(Game &game);

		void update(const std::string& gameState);

        static float gfxToGame(float);
        static float gameToGfx(float);
        
        void disableUserInput();
        
	private:

		void proceedEvents();
		void draw(const std::string &state);
		void drawBackground();
		void drawState(const std::string &state);

		Game &m_game;
		sf::RenderWindow m_window;
		bool m_disableUserInput;
};

#endif /* __VIEW_H__ */

