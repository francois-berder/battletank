#ifndef __COLLIDABLE_ENTITY_H__
#define __COLLIDABLE_ENTITY_H__

#include "Entity.hpp"

class Bullet;
class Tank;
class Obstacle;

class CollidableEntity : public Entity
{
	public:

		CollidableEntity(GameWorld &world, const EntityID id);
		virtual ~CollidableEntity() = default;

		virtual void handleCollision(CollidableEntity &b);
		virtual void handleCollision(Bullet &b);
		virtual void handleCollision(Tank &b);
		virtual void handleCollision(Obstacle &b);
};

#endif /* __COLLIDABLE_ENTITY_H__ */

