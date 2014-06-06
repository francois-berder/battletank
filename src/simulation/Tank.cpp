#include <sstream>

#include "Tank.hpp"


Tank::Tank(const EntityID id):
Entity(id),
m_health(0)
{
}

void Tank::update()
{

}

std::string Tank::log()
{
    std::stringstream ss;
    ss << "type:tank,";
    ss << "health:";
    ss << m_health;
    return Entity::log() + "," + ss.str();
}
