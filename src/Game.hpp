#ifndef __GAME_H__
#define __GAME_H__

#include "Options.hpp"
#include "GameWorld.hpp"


class Game
{
    public :
    
        Game();

        void setOptions(Options& opt);
        
        void run();
        
        void exit();
        
    private :
    
        void runInteractiveMode();
        void runNonInteractiveMode();
        
        bool m_isInteractive;
        bool m_exit;
        GameWorld m_gameWorld;
};

#endif /* __GAME_H__ */

