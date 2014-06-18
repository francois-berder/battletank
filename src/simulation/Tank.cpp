#include <sstream>
#include <stdexcept>

#include "Tank.hpp"
#include "Change.hpp"
#include "PhysicWorld.hpp"
#include "Logger.hpp"


Tank::Tank(const EntityID id, const b2Vec2& startPos):
Entity(id),
m_health(100),
m_body(nullptr, PhysicWorld::destroyBody)
{
    b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = startPos;
    b2Body *body = PhysicWorld::createBodyFromDef(&bodyDef);
    if(body == nullptr)
    {
        std::stringstream msg;
        msg << "Could not create physic body for tank ";
        msg << id;
        msg << ".\n";
        throw std::runtime_error(msg.str());
    }
    
    b2PolygonShape shape;
    shape.SetAsBox(0.15f, 0.15f);
    body->CreateFixture(&shape, 1.0f);
    body->SetUserData(this);
    m_body = std::unique_ptr<b2Body, void (*)(b2Body*)>(body, PhysicWorld::destroyBody);
}

void Tank::update()
{

}

void Tank::applyChange(const Change &change)
{
    if(getID() != change.getTargetID())
        throw std::runtime_error("Delivered change to wrong entity.");
    
    if(change.getName() == "move")
    {
        std::string dirName = change.getArg();
        b2Vec2 dir;
        if(dirName == "down")
            dir = b2Vec2(0.f, -2.f);
        else if(dirName == "up")
            dir = b2Vec2(0.f, 2.f);
        else if(dirName == "left")
            dir = b2Vec2(-2.f, 0.f);
        else if(dirName == "right")
            dir = b2Vec2(2.f, 0.f);
        else
            Logger::instance() << "Ignored move change, could not recognize direction " << change.getArg() << '\n';
        m_body->SetLinearVelocity(dir);
    }
    else
        Logger::instance() << "Ignored change " << change.getName() << '\n';
}

std::string Tank::print()
{
    b2Vec2 pos = m_body->GetPosition();
    std::stringstream ss;
    ss << "type:tank,";
    ss << "health:";
    ss << m_health;
    ss << ",pos:(";
    ss << pos.x;
    ss << ",";
    ss << pos.y;
    ss << ")";
    return Entity::print() + "," + ss.str();
}
