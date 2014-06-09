#include <sstream>

#include "EntityFactory.hpp"
#include "GameWorld.hpp"


GameWorld::GameWorld():
m_currentStep(0),
m_entities(),
m_factory()
{
}
        
void GameWorld::step()
{
    ++m_currentStep;
    
    for(auto it = m_entities.begin();
        it != m_entities.end();
        ++it)
        it->second->update();
}

void GameWorld::applyChange(const Change &change)
{
    const EntityID id = change.getTargetID();
    if(id != 0)
        m_entities[id]->applyChange(change);
        
    else
        proceedChange(change.getName(), change.getArg());
}

void GameWorld::proceedChange(const std::string &name, const std::string &arg)
{
    if(name == "new")
    {
        EntityPtr e = m_factory.createFromName(arg);
        m_entities[e->getID()] = e;
    }
    else if(name == "delete")
    {
        // Convert string to EntityID
        std::stringstream ss;
        ss << arg;
        EntityID id;
        ss >> id;
        
        m_entities.erase(id);
    }
}

bool GameWorld::isFinished()
{
    return false;
}

std::string GameWorld::print()
{
    std::stringstream ss;
    ss << "step " << m_currentStep << ":\n";
    ss << '[';
    for(auto it = m_entities.begin();
        it != m_entities.end();
        ++it)
        ss << it->second->print();
    ss << ']';
    return ss.str();
}   
