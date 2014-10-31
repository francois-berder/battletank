#ifndef __TANK_H__
#define __TANK_H__

#include <Box2D/Box2D.h>

#include "CollidableEntity.hpp"


class Tank : public CollidableEntity
{
	public:

        Tank(GameWorld &world, const EntityID id, const std::string& name, const b2Vec2& startPos);

		virtual void update();
		virtual void applyChange(const Change &change);
		virtual std::string print();

		virtual void handleCollision(CollidableEntity &b);
		virtual void handleCollision(Bullet &b);
		virtual void handleCollision(Tank &b);
		virtual void handleCollision(Obstacle &b);

        bool isDestroyed() const;
        std::string getName() const;

	private:

        std::string m_name;
		int m_health;
		std::unique_ptr<b2Body, void (*)(b2Body*)> m_body;
		float m_angularVelocity;
		float m_velocity;
		float m_cannonAngle;
};

#endif /* __TANK_H__ */

