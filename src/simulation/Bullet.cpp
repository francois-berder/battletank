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
m_dir(),
m_isDestroyed(false)
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
    m_dir.x = static_cast<float>(cos(angle));
    m_dir.y = static_cast<float>(sin(angle));
    m_dir.Normalize();
}

void Bullet::update()
{
    b2Vec2 pos = m_body->GetPosition();
    if(pos.x < -1.f || pos.x > 21.f     // Check if bullet is outside game space
    || pos.y < -1.f || pos.y > 16.f)
        destroy();
    else
    {
        const float speed = 15.f;
        m_body->SetLinearVelocity(speed * m_dir);
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
    destroy();
}

void Bullet::handleCollision(Tank &b)
{
    destroy();
}

void Bullet::handleCollision(Obstacle &b)
{
    destroy();
}

void Bullet::destroy()
{
    if(m_isDestroyed)
        return;

    m_isDestroyed = true;

    std::list<std::string> args;
    args.push_back(toString(getID()));
    Change c(GameWorld::getID(), "delete", args);
    getWorld().applyChange(c);
}

