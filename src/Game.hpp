#ifndef __GAME_H__
#define __GAME_H__

#include "Options.hpp"

class Game
{
    public :
    
        Game();

        void setOptions(Options& opt);
        
        void run();
        
    private :
    
        void runInteractiveMode();
        void runNonInteractiveMode();
        
        bool m_isInteractive;
};

#endif /* __GAME_H__ */

