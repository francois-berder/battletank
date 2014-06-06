#ifndef __TANK_H__
#define __TANK_H__

#include "Entity.hpp"

class Tank : public Entity
{
    public :
        
        Tank(const EntityID id);        
        
        virtual void update(); 
        
        virtual std::string log();

    private :
    
        int m_health;
};

#endif /* __TANK_H__ */

