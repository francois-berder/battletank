#include <vector>
#include <sstream>

#include "CommandFactory.hpp"


namespace
{

std::vector<std::string> split(const std::string &s) 
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, ' '))
        elems.push_back(item);

    return elems;
}

bool isEmptyString(const std::string &s)
{
    if(s.empty())
        return true;
        
    for(unsigned int i = 0; i < s.size(); ++i)
        if(s[i] != ' ')
            return false;
    
    return true;
}

}

CommandFactory::CommandFactory():
m_lastCmd(CommandPtr(new Command))
{
}

CommandPtr CommandFactory::parseCmd(const std::string &cmdStr)
{
    if(isEmptyString(cmdStr))
        return m_lastCmd;

    std::vector<std::string> args = split(cmdStr);
    
    std::string cmdName = args[0];
    
    CommandPtr cmd;
    if(cmdName == "q" || cmdName == "quit")
        cmd = CommandPtr(new QuitCommand);
    else if(cmdName == "s" || cmdName == "step")
        cmd = CommandPtr(new StepCommand);
    else if(cmdName == "h" || cmdName == "help")
        cmd = CommandPtr(new HelpCommand);
    else if(cmdName == "p" || cmdName == "print")
        cmd = CommandPtr(new PrintCommand);
    else
        cmd = CommandPtr(new InvalidCommand);
        
    m_lastCmd = cmd;
    
    return cmd;
}


