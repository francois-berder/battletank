#ifndef __TANK_H__
#define __TANK_H__

#include <Box2D/Box2D.h>

#include "Entity.hpp"


class Tank : public Entity
{
    public :
        
        Tank(const EntityID id);        
        
        virtual void update(); 
        
        virtual std::string print();

    private :
    
        int m_health;
        std::unique_ptr<b2Body, void (*)(b2Body*) > m_body;
};

#endif /* __TANK_H__ */

