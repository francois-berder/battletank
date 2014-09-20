#include "CollidableEntity.hpp"

CollidableEntity::CollidableEntity(GameWorld &world, const EntityID id) :
		Entity(world, id)
{
}

void CollidableEntity::handleCollision(CollidableEntity &b)
{

}

void CollidableEntity::handleCollision(Bullet &b)
{

}

void CollidableEntity::handleCollision(Obstacle &b)
{

}

void CollidableEntity::handleCollision(Tank &b)
{

}


