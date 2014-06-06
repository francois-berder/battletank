#ifndef __ENTITY_FACTORY_H__
#define __ENTITY_FACTORY_H__

#include <string>

#include "Entity.hpp"

class EntityFactory
{
    public :
    
        EntityFactory();
        
        EntityPtr createFromName(const std::string &name);
        
    private :

        EntityID m_currentID;
};

#endif /* __ENTITY_FACTORY_H__ */

