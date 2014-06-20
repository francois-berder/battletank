#include <sstream>
#include <stdexcept>

#include "Tank.hpp"
#include "Change.hpp"
#include "PhysicWorld.hpp"
#include "Logger.hpp"


Tank::Tank(const EntityID id, const b2Vec2& startPos):
Entity(id),
m_health(100),
m_body(nullptr, PhysicWorld::destroyBody),
m_angularVelocity(0.f),
m_velocity(0.f)
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
    if(fabs(m_angularVelocity) > 0.1f)
    {
        m_body->SetAngularVelocity(m_angularVelocity);
        m_angularVelocity = 0.f;
    }

    if(fabs(m_velocity) > 0.1f)
    {
        b2Vec2 vel(0.f, m_velocity);
        vel = b2Mul(b2Rot(m_body->GetAngle()), vel);
        m_body->SetLinearVelocity(vel);
        m_velocity = 0.f;
    }
}

void Tank::applyChange(const Change &change)
{
    if(getID() != change.getTargetID())
        throw std::runtime_error("Delivered change to wrong entity.");
    
    if(change.getName() == "move")
    {
        std::string dirName = change.getArg();
        if(dirName == "down")
            m_velocity -= 2.f;
        else if(dirName == "up")
            m_velocity += 2.f;
        else if(dirName == "left")
            m_angularVelocity -= 2.f;
        else if(dirName == "right")
            m_angularVelocity += -2.f;
        else
            Logger::instance() << "Ignored move change, could not recognize direction " << change.getArg() << '\n';
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
    ss << ",pos:[x:";
    ss << pos.x;
    ss << ",y:";
    ss << pos.y;
    ss << "],angle:";
    ss << m_body->GetAngle();
    return Entity::print() + "," + ss.str();
}
