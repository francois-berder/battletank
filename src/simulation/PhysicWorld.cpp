#include <cassert>

#include "PhysicWorld.hpp"

#define TIME_STEP (1.0f/60.0f)
#define VELOCITY_ITER 6
#define POSITION_ITER 2

namespace
{

PhysicWorld& instance()
{
	static PhysicWorld pw;
	return pw;
}

}

PhysicWorld::PhysicWorld() :
		m_world(b2Vec2(0, 0)), m_contactListener(new ContactListener())
{
	m_world.SetContactListener(m_contactListener.get());
}

void PhysicWorld::update()
{
	PhysicWorld &pw = instance();
	pw.m_world.ClearForces();
	pw.m_world.Step(TIME_STEP, VELOCITY_ITER, POSITION_ITER);
}

b2Body* PhysicWorld::createBodyFromDef(b2BodyDef *def)
{
	assert(def != nullptr);

	return instance().m_world.CreateBody(def);
}

void PhysicWorld::destroyBody(b2Body *body)
{
	assert(body != nullptr);

	instance().m_world.DestroyBody(body);
}

