#ifndef __CHANGE_H__
#define __CHANGE_H__

#include <string>
#include <list>

#include "Entity.hpp"
#include "Argument.hpp"


class Change
{
    public :
    
        Change() = delete;
        Change(const EntityID id, const std::string &name, const std::list<Argument> &args);
        
        EntityID getTargetID() const;
        std::string getName() const;
        std::list<Argument> getArgs() const;
        
    private :
    
        const EntityID m_id;
        const std::string m_name;
        const std::list<Argument> m_args;
};

#endif /* __CHANGE_H__ */

