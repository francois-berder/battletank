#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include <Box2D/Box2D.h>
#include <memory>

#include "CollidableEntity.hpp"


class Obstacle : public CollidableEntity
{
	public:

		Obstacle(GameWorld &world, const EntityID id, const b2Vec2 &pos);

		virtual std::string print();

		virtual void handleCollision(CollidableEntity &b);
		virtual void handleCollision(Bullet &b);
		virtual void handleCollision(Tank &b);
		virtual void handleCollision(Obstacle &b);

	private:

		std::unique_ptr<b2Body, void (*)(b2Body*)> m_body;
};

#endif /* __OBSTACLE_H__ */

