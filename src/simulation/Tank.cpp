#include <sstream>

#include "Tank.hpp"


Tank::Tank(const EntityID id):
Entity(id),
m_health(100)
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
