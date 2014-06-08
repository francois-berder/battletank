#ifndef __COMMAND_FACTORY_H__
#define __COMMAND_FACTORY_H__

#include <string>

#include "Command.hpp"

class CommandFactory
{
    public :
        
        CommandFactory();
        
        CommandPtr parseCmd(const std::string &cmdStr);
        
    private :
    
        CommandPtr m_lastCmd;
};

#endif /* __COMMAND_FACTORY_H__ */

