#include <sstream>

#include "Bullet.hpp"
#include "PhysicWorld.hpp"
#include "Change.hpp"
#include "GameWorld.hpp"
#include "Utils.hpp"


Bullet::Bullet(GameWorld &world, const EntityID id, const EntityID tankID, const b2Vec2 &pos, float angle):
CollidableEntity(world, id),
m_tankID(tankID),
m_body(nullptr, PhysicWorld::destroyBody),
m_dir()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;
    bodyDef.angle = angle;
	b2Body *body = PhysicWorld::createBodyFromDef(&bodyDef);
	if(body == nullptr)
	{
		std::stringstream msg;
		msg << "Could not create physic body for bullet ";
		msg << id;
		msg << ".\n";
		throw std::runtime_error(msg.str());
	}

	b2PolygonShape shape;
	shape.SetAsBox(0.1375f, 0.05f);
	body->CreateFixture(&shape, 1.0f);
	body->SetUserData(this);
	m_body = std::unique_ptr<b2Body, void (*)(b2Body*)>(body,
			PhysicWorld::destroyBody);
    
    angle += 3.14f / 2.f;
    m_dir.x = cos(angle);
    m_dir.y = sin(angle);
    m_dir.Normalize();
}

void Bullet::update()
{
    const float speed = 15.f;
    m_body->SetLinearVelocity(speed * m_dir);

    b2Vec2 pos = m_body->GetPosition();
    
    // Check if bullet is outside game space
    if(pos.x < -1.f || pos.x > 21.f
    || pos.y < -1.f || pos.y > 16.f)
    {
        std::list<std::string> args;
        args.push_back(toString(getID()));
        Change c(GameWorld::getID(), "delete", args);
        getWorld().applyChange(c);
    }
}

std::string Bullet::print()
{
	b2Vec2 pos = m_body->GetPosition();
	std::stringstream ss;
	ss << "type:bullet";
	ss << ",pos:{x:";
	ss << pos.x;
	ss << ",y:";
	ss << pos.y;
	ss << "},angle:";
	float angle = m_body->GetAngle();
	angle = convertRadToDeg(angle);
	ss << static_cast<int>(angle);
	
	return Entity::print() + "," + ss.str();
}

void Bullet::handleCollision(CollidableEntity &b)
{
    b.handleCollision(*this);
}


void Bullet::handleCollision(Bullet &b)
{
    std::list<std::string> args;
    args.push_back(toString(getID()));
    Change c(GameWorld::getID(), "delete", args);
    getWorld().applyChange(c);

    // TODO: add explosion
}

void Bullet::handleCollision(Tank &b)
{
    std::list<std::string> args;
    args.push_back(toString(getID()));
    Change c(GameWorld::getID(), "delete", args);
    getWorld().applyChange(c);

    // TODO: add explosion
}

void Bullet::handleCollision(Obstacle &b)
{
    std::list<std::string> args;
    args.push_back(toString(getID()));
    Change c(GameWorld::getID(), "delete", args);
    getWorld().applyChange(c);

    // TODO: add explosion
}
