#ifndef __BULLET_HPP__
#define __BULLET_HPP__

#include <Box2D/Box2D.h>

#include "CollidableEntity.hpp"

class Bullet : public CollidableEntity
{
    public :

        Bullet(GameWorld &world, const EntityID id, const EntityID tankID, const b2Vec2 &pos, float angle);

		virtual void update();
		virtual std::string print();

		virtual void handleCollision(CollidableEntity &b);
		virtual void handleCollision(Bullet &b);
		virtual void handleCollision(Tank &b);
		virtual void handleCollision(Obstacle &b);

    private :

        EntityID m_tankID;
		std::unique_ptr<b2Body, void (*)(b2Body*)> m_body;        
        b2Vec2 m_dir;
};

#endif /* __BULLET_HPP__ */

