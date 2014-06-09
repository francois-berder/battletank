#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include <Box2D/Box2D.h>

#include "Entity.hpp"


class Obstacle : public Entity
{
    public :
    
        Obstacle(const EntityID id);
    
        virtual std::string print();
        
    private :
    
        b2Body *m_body;
};

#endif /* __OBSTACLE_H__ */

