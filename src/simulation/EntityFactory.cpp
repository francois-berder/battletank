#include <stdexcept>
#include <sstream>

#include "EntityFactory.hpp"
#include "Tank.hpp"
#include "Obstacle.hpp"


EntityFactory::EntityFactory():
m_currentID(0)
{
}
        
EntityPtr EntityFactory::createFromName(const std::string &name)
{
    ++m_currentID;
    
    if(name == "tank")
        return EntityPtr(new Tank(m_currentID)); 
    else if(name == "obstacle")
        return EntityPtr(new Obstacle(m_currentID));
    else
    {
        std::stringstream msg;
        msg << "Could not create entity from name: ";
        msg << name;
        msg << ".\n";
        throw std::runtime_error(msg.str());
    }
}
