#include <sstream>

#include "Explosion.hpp"
#include "GameWorld.hpp"
#include "Utils.hpp"


Explosion::Explosion(GameWorld &world, const EntityID id, const float x, const float y):
Entity(world, id),
m_x(x),
m_y(y),
m_timer(64)
{
}

void Explosion::update()
{
    // Remove explosion entity if timer reaches 0
    if(m_timer == 0)
    {
        std::list<std::string> args;
        args.push_back(toString(getID()));
        Change c(GameWorld::getID(), "delete", args);
        getWorld().applyChange(c);
    }
    else
        --m_timer;
}

std::string Explosion::print()
{
    std::stringstream ss;
    ss << "type:explosion,pos:{";
    ss << "x:" << m_x;
    ss << ",y:" << m_y;
    ss << "}";
    ss << ",timer:" << m_timer;
	return Entity::print() + "," + ss.str();
}
