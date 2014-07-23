#include <stdexcept>
#include <sstream>

#include "Obstacle.hpp"
#include "PhysicWorld.hpp"

Obstacle::Obstacle(const EntityID id, const b2Vec2 &pos) :
		Entity(id), m_body(nullptr, PhysicWorld::destroyBody)
{

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = pos;
	b2Body *body = PhysicWorld::createBodyFromDef(&bodyDef);
	if(body == nullptr)
	{
		std::stringstream msg;
		msg << "Could not create physic body for obstacle ";
		msg << id;
		msg << ".\n";
		throw std::runtime_error(msg.str());
	}

	b2PolygonShape shape;
	shape.SetAsBox(0.1f, 0.1f);
	body->CreateFixture(&shape, 1.0f);
	body->SetUserData(this);
	m_body = std::unique_ptr<b2Body, void (*)(b2Body*)>(body,
			PhysicWorld::destroyBody);
}

std::string Obstacle::print()
{
	b2Vec2 pos = m_body->GetPosition();
	std::stringstream ss;
	ss << "type:obstacle,pos:{x:";
	ss << pos.x;
	ss << ",y:";
	ss << pos.y;
	ss << "}";
	return Entity::print() + "," + ss.str();
}

