#include <cassert>

#include "ContactListener.hpp"
#include "CollidableEntity.hpp"


void ContactListener::BeginContact(b2Contact *contact)
{
    assert(contact->GetFixtureA()->GetBody()->GetUserData() != nullptr 
    && contact->GetFixtureB()->GetBody()->GetUserData() != nullptr);
    
    CollidableEntity *a = reinterpret_cast<CollidableEntity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    CollidableEntity *b = reinterpret_cast<CollidableEntity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    a->handleCollision(*b);
    b->handleCollision(*a);
}

