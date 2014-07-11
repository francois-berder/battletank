#ifndef __TANK_H__
#define __TANK_H__

#include <Box2D/Box2D.h>

#include "Entity.hpp"

class Tank : public Entity
{
	public:

		Tank(const EntityID id, const b2Vec2& startPos);

		virtual void update();
		virtual void applyChange(const Change &change);
		virtual std::string print();

	private:

		int m_health;
		std::unique_ptr<b2Body, void (*)(b2Body*)> m_body;
		float m_angularVelocity;
		float m_velocity;
};

#endif /* __TANK_H__ */

