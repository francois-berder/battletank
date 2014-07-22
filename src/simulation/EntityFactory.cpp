#include <stdexcept>
#include <sstream>

#include "EntityFactory.hpp"
#include "Tank.hpp"
#include "Obstacle.hpp"
#include "Utils.hpp"

EntityFactory::EntityFactory() :
		m_currentID(0)
{
}

EntityPtr EntityFactory::createFromName(const std::string &name,
		std::list<std::string> args)
{
	++m_currentID;

	if(name == "tank")
	{
		if(args.size() < 2)
			throw std::runtime_error(
					"Not enough arguments given to create tank.\n");

		float x = toFloat(args.front());
		args.pop_front();
		float y = toFloat(args.front());
		return EntityPtr(new Tank(m_currentID, b2Vec2(x, y)));
	}
	else if(name == "obstacle")
	{
		if(args.size() < 2)
			throw std::runtime_error(
					"Not enough arguments given to create obstacle.\n");

		float x = toFloat(args.front());
		args.pop_front();
		float y = toFloat(args.front());
		return EntityPtr(new Obstacle(m_currentID, b2Vec2(x, y)));
	}
	else
	{
		std::stringstream msg;
		msg << "Could not create entity from name: ";
		msg << name;
		msg << ".\n";
		throw std::runtime_error(msg.str());
	}
}
