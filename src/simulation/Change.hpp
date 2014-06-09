#ifndef __CHANGE_H__
#define __CHANGE_H__

#include <string>

#include "Entity.hpp"

class Change
{
    public :
    
        Change() = delete;
        Change(const EntityID id, const std::string &name, const std::string &arg);
        
        EntityID getTargetID() const;
        std::string getName() const;
        std::string getArg() const;
        
    private :
    
        const EntityID m_id;
        const std::string m_name;
        const std::string m_arg;
};

#endif /* __CHANGE_H__ */

