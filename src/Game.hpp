#ifndef __GAME_H__
#define __GAME_H__

#include "Option.hpp"
#include "GameWorld.hpp"
#include "View.hpp"


class Game
{
    public :
    
        Game();

        void setOptions(std::list<Option>& options);
        
        void run();
        
        void exit();
        
        GameWorld& getWorld();
        
    private :
    
        void runInteractiveMode();
        void runNonInteractiveMode();
        
        void executeFile(const std::string& path);
        
        bool m_isInteractive;
        bool m_exit;
        GameWorld m_gameWorld;
        View m_view;
};

#endif /* __GAME_H__ */

