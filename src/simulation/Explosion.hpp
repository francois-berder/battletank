#ifndef __EXPLOSION_HPP__
#define __EXPLOSION_HPP__


#include "Entity.hpp"

class Explosion : public Entity
{
    public :

		Explosion(GameWorld &world, const EntityID id, const float x, const float y);

		virtual void update();

		virtual std::string print();

    private :

        const float m_x, m_y;
        unsigned int m_timer;
};

#endif /* __EXPLOSION_HPP__ */

