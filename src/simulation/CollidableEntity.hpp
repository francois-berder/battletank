#ifndef __COLLIDABLE_ENTITY_H__
#define __COLLIDABLE_ENTITY_H__

#include "Entity.hpp"

class CollidableEntity : public Entity
{
	public:

		CollidableEntity(GameWorld &world, const EntityID id);
		virtual ~CollidableEntity() = default;

		virtual void handleCollision(const CollidableEntity &b);
};

#endif /* __COLLIDABLE_ENTITY_H__ */

