#ifndef __COMMAND_FACTORY_H__
#define __COMMAND_FACTORY_H__

#include <string>

#include "Command.hpp"

class CommandFactory
{
    public :
        
        CommandFactory(Game &game);
        
        CommandPtr parseCmd(const std::string &cmdStr);
        
    private :
    
        std::string m_lastCmd;
        Game &m_game;
};

#endif /* __COMMAND_FACTORY_H__ */

