#ifndef __PHYSIC_WORLD_H__
#define __PHYSIC_WORLD_H__

#include <Box2D/Box2D.h>
#include <memory>

#include "ContactListener.hpp"

class PhysicWorld
{
	public:

		PhysicWorld();

		static void update();
		static b2Body* createBodyFromDef(b2BodyDef *def);
		static void destroyBody(b2Body *body);

	private:

		b2World m_world;
		std::unique_ptr<b2ContactListener> m_contactListener;
};

#endif /* __PHYSIC_WORLD_H__ */
