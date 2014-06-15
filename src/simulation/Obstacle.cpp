#include "Obstacle.hpp"
#include "PhysicWorld.hpp"


Obstacle::Obstacle(const EntityID id):
Entity(id),
m_body(nullptr, PhysicWorld::destroyBody)
{
}
        
std::string Obstacle::print()
{
    return Entity::print() + ",type:obstacle";
}

