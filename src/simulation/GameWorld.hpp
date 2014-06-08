#ifndef __GAME_WORLD_H__
#define __GAME_WORLD_H__

#include <list>

#include "Entity.hpp"


class GameWorld
{
    public :
    
        GameWorld();
        
        void step();
        
        bool isFinished();
        
        std::string print();
        
    private :
    
        unsigned int m_currentStep;
        std::list<EntityPtr> m_entities;
};

#endif /* __GAME_WORLD_H__ */

