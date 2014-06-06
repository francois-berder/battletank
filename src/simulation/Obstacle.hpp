#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "Entity.hpp"


class Obstacle : public Entity
{
    public :
    
        Obstacle(const EntityID id);
    
        virtual std::string log();
};

#endif /* __OBSTACLE_H__ */

