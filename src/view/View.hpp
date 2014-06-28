#ifndef __VIEW_H__
#define __VIEW_H__

#include <SFML/Graphics.hpp>


class Game;

class View
{
    public :

        View(Game &game);

        void update();

    private :
    
        Game &m_game;
        sf::RenderWindow m_window;
};

#endif /* __VIEW_H__ */

