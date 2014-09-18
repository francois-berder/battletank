#include "CollidableEntity.hpp"

CollidableEntity::CollidableEntity(GameWorld &world, const EntityID id) :
		Entity(world, id)
{
}

void CollidableEntity::handleCollision(const CollidableEntity &b)
{

}

