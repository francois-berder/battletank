#include <stdexcept>
#include <sstream>

#include "EntityFactory.hpp"
#include "Tank.hpp"
#include "Obstacle.hpp"
#include "Bullet.hpp"
#include "Utils.hpp"

EntityFactory::EntityFactory(GameWorld &world) :
m_world(world),
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
		return EntityPtr(new Tank(m_world, m_currentID, b2Vec2(x, y)));
	}
	else if(name == "obstacle")
	{
		if(args.size() < 2)
			throw std::runtime_error(
					"Not enough arguments given to create obstacle.\n");

		float x = toFloat(args.front());
		args.pop_front();
		float y = toFloat(args.front());
		return EntityPtr(new Obstacle(m_world, m_currentID, b2Vec2(x, y)));
	}
    else if(name == "bullet")
    {
        if(args.size() < 4)
			throw std::runtime_error(
					"Not enough arguments given to create bullet.\n");

        EntityID tankID = toUInteger(args.front());
        args.pop_front();
        b2Vec2 pos;
        pos.x = toFloat(args.front());
        args.pop_front();
        pos.y = toFloat(args.front());
        args.pop_front();
        float angle = toFloat(args.front());

		return EntityPtr(new Bullet(m_world, m_currentID, tankID, pos, angle));
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
