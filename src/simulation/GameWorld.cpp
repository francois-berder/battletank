#include <stdexcept>
#include <sstream>

#include "EntityFactory.hpp"
#include "GameWorld.hpp"
#include "Utils.hpp"
#include "Logger.hpp"

GameWorld::GameWorld() :
m_currentStep(0),
m_entities(),
m_toRemove(),
m_factory(*this)
{
}

void GameWorld::step()
{
	++m_currentStep;

	for(auto ety : m_entities)
		ety.second->update();

	PhysicWorld::update();

    m_toRemove.clear();
}

void GameWorld::applyChange(const Change &change)
{    
	const EntityID id = change.getTargetID();
	if(id != getID())
		m_entities[id]->applyChange(change);
	else
		proceedChange(change.getName(), change.getArgs());
}

void GameWorld::proceedChange(const std::string &name,
		const std::list<std::string> &args)
{
	if(name == "new")
	{
		if(args.empty())
			throw std::runtime_error("Missing type to create entity.\n");

		std::string type = args.front();
		std::list<std::string> entityArgs = args;
		entityArgs.pop_front();
		EntityPtr e = m_factory.createFromName(type, entityArgs);
		m_entities[e->getID()] = e;
	}
	else if(name == "delete")
	{
		if(args.empty())
			throw std::runtime_error("Missing entity ID to delete entity.\n");

		EntityID id = toUInteger(args.front());
        auto it = m_entities.find(id);
        if(it == m_entities.end())
            Logger::warning() << "Tried to delete unknown entity (id=" << it->first << ")\n";
        else
        {
            // Keep a list of deleted entities such that their destructor
            // is called only at the end of GameWorld::update when we clear
            // this list.
            m_toRemove.push_back(m_entities[id]);
		    m_entities.erase(id);
        }
	}
	else
		Logger::warning() << "Ignored change " << name << '\n';
}

bool GameWorld::isFinished()
{
	return false;
}

std::string GameWorld::print() const
{
	std::stringstream ss;
	ss << '[';
	for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
	    ss << '{';
		ss << it->second->print();
		ss << '}';
		if(std::next(it) != m_entities.end())
			ss << ';';
	}
	ss << ']';
	return ss.str();
}

std::string GameWorld::printEntity(EntityID id) const
{
	auto it = m_entities.find(id);
	if(it == m_entities.end())
	{
		std::stringstream msg;
		msg << "Cannot print unknown entity ";
		msg << id;
		throw std::runtime_error(msg.str());
	}

	return it->second->print();
}

unsigned int GameWorld::getCurrentStep() const
{
	return m_currentStep;
}

unsigned int GameWorld::getID()
{
    return 0;
}

