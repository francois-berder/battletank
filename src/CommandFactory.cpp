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
    {
        int nbSteps = 1;
        if(args.size() > 1)
        {
            std::stringstream ss;
            ss << args[1];
            ss >> nbSteps;
        }
        
        if(nbSteps < 0)
            cmd = CommandPtr(new InvalidCommand);
        else
            cmd = CommandPtr(new StepCommand(nbSteps));
    }
    else if(cmdName == "h" || cmdName == "help")
        cmd = CommandPtr(new HelpCommand);
    else if(cmdName == "p" || cmdName == "print")
        cmd = CommandPtr(new PrintCommand);
    else if(cmdName == "a" || cmdName == "apply")
    {
        if(args.size() < 4)
            cmd = CommandPtr(new InvalidCommand);
        else
        {
            EntityID id;
            std::stringstream ss;
            ss << args[1];
            ss >> id;
            std::string name = args[2];
            std::string arg = args[3];
            cmd = CommandPtr(new ApplyCommand(id, name, arg));
        }    
    }
    else
        cmd = CommandPtr(new InvalidCommand);
        
    m_lastCmd = cmd;
    
    return cmd;
}


