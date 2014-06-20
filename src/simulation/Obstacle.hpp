#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include <Box2D/Box2D.h>
#include <memory>

#include "Entity.hpp"


class Obstacle : public Entity
{
    public :
    
        Obstacle(const EntityID id, const b2Vec2 &pos);
    
        virtual std::string print();
        
    private :
    
        std::unique_ptr<b2Body, void (*)(b2Body*)> m_body;
};

#endif /* __OBSTACLE_H__ */

