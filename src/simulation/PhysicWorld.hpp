#ifndef __PHYSIC_WORLD_H__
#define __PHYSIC_WORLD_H__

#include <Box2D/Box2D.h>

#include "ContactListener.hpp"


class PhysicWorld
{
    public :
    
        static PhysicWorld& instance();
        
        void update();
        b2Body* createBodyFromDef(b2BodyDef *def);
        void destroyBody(b2Body *body);
        
    private :
    
        PhysicWorld();
        PhysicWorld(const PhysicWorld &pw);
        virtual ~PhysicWorld();
        
        PhysicWorld& operator=(const PhysicWorld &pw);
        
        
        b2World m_world;     
        b2ContactListener *m_contactListener;   
};

#endif /* __PHYSIC_WORLD_H__ */
