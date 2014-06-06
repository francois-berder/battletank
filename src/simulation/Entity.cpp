#include <sstream>

#include "Entity.hpp"


Entity::Entity(const EntityID id):
m_id(id)
{
}
       
Entity::~Entity()
{
}

void Entity::update()
{
    // by default, do nothing
}

std::string Entity::log()
{
    std::stringstream ss;
    ss << "id:";
    ss << m_id;
    return ss.str();
}

