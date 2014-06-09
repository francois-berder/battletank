#ifndef __GAME_WORLD_H__
#define __GAME_WORLD_H__

#include <map>

#include "Entity.hpp"
#include "EntityFactory.hpp"
#include "Change.hpp"


class GameWorld
{
    public :
    
        GameWorld();
        
        void step();
        
        void applyChange(const Change &change);
        
        bool isFinished();
        
        std::string print();
        
    private :
    
        void proceedChange(const std::string &name, const std::string &arg);
    
        unsigned int m_currentStep;
        std::map<EntityID, EntityPtr> m_entities;
        EntityFactory m_factory;
};

#endif /* __GAME_WORLD_H__ */

