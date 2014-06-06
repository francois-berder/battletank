#include "Obstacle.hpp"

Obstacle::Obstacle(const EntityID id):
Entity(id)
{
}
        
std::string Obstacle::log()
{
    return Entity::log() + ",type:obstacle";
}

