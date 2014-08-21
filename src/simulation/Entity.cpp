#include <sstream>

#include "Entity.hpp"
#include "Change.hpp"
#include "Logger.hpp"

Entity::Entity(const EntityID id) :
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

void Entity::applyChange(const Change &change)
{
	Logger::warning() << "Ignored " << change.getName() << " change.\n";
}

std::string Entity::print()
{
	std::stringstream ss;
	ss << "id:";
	ss << m_id;
	return ss.str();
}

EntityID Entity::getID() const
{
	return m_id;
}

