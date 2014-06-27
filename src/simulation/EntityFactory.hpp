#ifndef __ENTITY_FACTORY_H__
#define __ENTITY_FACTORY_H__

#include <string>
#include <list>

#include "Entity.hpp"
#include "Argument.hpp"


class EntityFactory
{
    public :

        EntityFactory();

        EntityPtr createFromName(const std::string &name, std::list<Argument> args = std::list<Argument>());

    private :

        EntityID m_currentID;
};

#endif /* __ENTITY_FACTORY_H__ */

