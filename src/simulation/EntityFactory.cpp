#include <stdexcept>
#include <sstream>

#include "EntityFactory.hpp"
#include "Tank.hpp"
#include "Obstacle.hpp"


namespace
{
    float convertFromStr(const std::string& str)
    {
        std::stringstream ss;
        ss << str;
        float val;
        ss >> val;
        return val;
    }
}

EntityFactory::EntityFactory():
m_currentID(0)
{
}
        
EntityPtr EntityFactory::createFromName(const std::string &name, std::list<std::string> args)
{
    ++m_currentID;
    
    if(name == "tank")
    {
        if(args.size() < 2)
            throw std::runtime_error("Not enough arguments given to create tank.\n");

        float x = convertFromStr(args.front());
        args.pop_front();
        float y = convertFromStr(args.front());
        return EntityPtr(new Tank(m_currentID, b2Vec2(x, y))); 
    }
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
