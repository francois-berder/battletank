#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <memory>
#include <string>

typedef unsigned int EntityID;

class Entity
{
    public :
       
       Entity(const EntityID id);
       virtual ~Entity();
       
       virtual void update();
       
       virtual std::string log();
       
    private :
   
        const EntityID m_id;
}; 

typedef std::shared_ptr<Entity> EntityPtr;

#endif /* __ENTITY_H__ */

