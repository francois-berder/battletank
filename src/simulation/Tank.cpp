#include <sstream>
#include <stdexcept>
#include <cmath>

#include "Tank.hpp"
#include "GameWorld.hpp"
#include "Change.hpp"
#include "PhysicWorld.hpp"
#include "Logger.hpp"
#include "Utils.hpp"


Tank::Tank(GameWorld &world, const EntityID id, const std::string& name, const b2Vec2& startPos) :
CollidableEntity(world, id),
m_name(name),
m_health(100),
m_body(nullptr, PhysicWorld::destroyBody),
m_angularVelocity(0.f),
m_velocity(0.f),
m_cannonAngle(0.f)
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
	shape.SetAsBox(0.7f, 0.825f);
	body->CreateFixture(&shape, 1.0f);
	body->SetUserData(this);
	m_body = std::unique_ptr<b2Body, void (*)(b2Body*)>(body,
			PhysicWorld::destroyBody);
}

void Tank::update()
{
    if(isDestroyed())
    {
    	m_angularVelocity = 0.f;
        m_velocity = 0.f;
    }

	m_body->SetAngularVelocity(m_angularVelocity);
	m_angularVelocity = 0.f;

	b2Vec2 vel(0.f, m_velocity);
	vel = b2Mul(b2Rot(m_body->GetAngle()), vel);
	m_body->SetLinearVelocity(vel);
	m_velocity = 0.f;
}

void Tank::applyChange(const Change &change)
{
	if(getID() != change.getTargetID())
		throw std::runtime_error("Delivered change to wrong entity.");

    if(isDestroyed())
        return;

    const std::string &changeName = change.getName();
	if(changeName == "move")
	{
		if(change.getArgs().empty())
			throw std::runtime_error(
					"Missing direction to apply move change to tank.");

		std::string dirName = change.getArgs().front();

		if(dirName == "down")
			m_velocity += 2.f;
		else if(dirName == "up")
			m_velocity -= 2.f;
		else if(dirName == "left")
			m_angularVelocity -= 2.f;
		else if(dirName == "right")
			m_angularVelocity += 2.f;
		else
			Logger::warning()
					<< "Ignored move change, could not recognize direction "
					<< dirName << '\n';
	}
	else if(changeName == "dir")
	{
		std::list<std::string> args = change.getArgs();
		if(args.size() < 2)
			throw std::runtime_error(
					"Missing coordinates to apply dir change to tank.");
        
        // b = (x,y)
		double x = toFloat(args.front());
		args.pop_front();
		double y = toFloat(args.front());

        // Relative to tank origin        
    	b2Vec2 posTank = m_body->GetPosition();
    	x -= posTank.x;
    	y -= posTank.y;

        // Normalize b
        double l = sqrt((x*x) + (y*y));
        x /= l;
        y /= l;

	    double angle = m_body->GetAngle();
	    double sign = x * cos(angle) + y * sin(angle);
	    // zero-angle is actually -90°, needs to compensate for it
	    angle += 3.14f/2.f;
        
	    // d = dot(a,b)
        double d = x * cos(angle) + y * sin(angle); 
        m_cannonAngle = convertRadToDeg(static_cast<float>(acos(d)));

        while(m_cannonAngle < 0.f)
            m_cannonAngle += 360.f;
        while(m_cannonAngle >= 360.f)
            m_cannonAngle -= 360.f;
        if(sign > 0.f)
            m_cannonAngle = 360.f - m_cannonAngle;
        m_cannonAngle += convertRadToDeg(m_body->GetAngle());
	}
    else if(changeName == "shoot")
    {
    	b2Vec2 pos = m_body->GetPosition();
        double angle = convertDegToRad(m_cannonAngle);
        double a = convertDegToRad(m_cannonAngle + 90);
        b2Vec2 dir(static_cast<float>(cos(a)), static_cast<float>(sin(a)));
        dir.Normalize();
        pos += 2.f*dir;

        std::list<std::string> args;
        args.push_back("bullet");
        args.push_back(toString(getID()));
        args.push_back(toString(pos.x));
        args.push_back(toString(pos.y));
        args.push_back(toString(angle));

        Change c(0, "new", args);
        getWorld().applyChange(c);
    }
	else
		Logger::warning() << "Ignored change " << change.getName() << '\n';
}

std::string Tank::print()
{ 
	b2Vec2 pos = m_body->GetPosition();
	std::stringstream ss;
	ss << "type:tank,";
	ss << "health:";
    ss << m_health;
	ss << ",pos:{x:";
	ss << pos.x;
	ss << ",y:";
	ss << pos.y;
	ss << "},angle:";
	float angle = m_body->GetAngle();
	angle = convertRadToDeg(angle);
	ss << static_cast<int>(angle);
	
	ss << ",cannonAngle:";
	ss << static_cast<int>(m_cannonAngle);
	
	return Entity::print() + "," + ss.str();
}

void Tank::handleCollision(CollidableEntity &b)
{
    b.handleCollision(*this);
}

void Tank::handleCollision(Bullet &b)
{
    if(m_health > 10)
        m_health -= 10;
    else
    {
        if(m_health > 0)
        {
            // add explosion
            std::list<std::string> args;
            args.push_back("explosion");
            b2Vec2 pos = m_body->GetPosition();
            args.push_back(toString(pos.x));
            args.push_back(toString(pos.y));
            Change c(GameWorld::getID(), "new", args);
            getWorld().applyChange(c);
        }
        m_health = 0;
    }
}

void Tank::handleCollision(Tank &b)
{
}

void Tank::handleCollision(Obstacle &b)
{
}

bool Tank::isDestroyed() const
{
    return m_health == 0;
}

std::string Tank::getName() const
{
    return m_name;
}
