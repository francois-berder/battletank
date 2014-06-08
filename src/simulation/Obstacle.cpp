#include "Obstacle.hpp"

Obstacle::Obstacle(const EntityID id):
Entity(id)
{
}
        
std::string Obstacle::print()
{
    return Entity::print() + ",type:obstacle";
}

