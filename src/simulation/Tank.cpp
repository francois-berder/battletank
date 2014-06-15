#include <sstream>

#include "Tank.hpp"
#include "PhysicWorld.hpp"

Tank::Tank(const EntityID id):
Entity(id),
m_health(100),
m_body(nullptr, PhysicWorld::destroyBody)
{
}

void Tank::update()
{

}

std::string Tank::print()
{
    std::stringstream ss;
    ss << "type:tank,";
    ss << "health:";
    ss << m_health;
    return Entity::print() + "," + ss.str();
}
