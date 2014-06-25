#include <list>
#include <sstream>

#include "CommandFactory.hpp"


namespace
{
    std::list<std::string> split(const std::string &s) 
    {
        std::list<std::string> elems;
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
    
    bool isInteger(const std::string &s)
    {
        for(unsigned int i = 0; i < s.size(); ++i)
            if(!('0' <= s[i] && s[i] <= '9'))
                return false;
        return true; 
    }
    
    std::string concat(const std::list<std::string> &s)
    {
        std::stringstream ss;
        for(auto it = s.begin(); it != s.end(); ++it)
        {
            ss << *it;
            ss << " ";
        }
        std::string str = ss.str();
        
        return str.substr(0, str.size()-1);
    }
}

CommandFactory::CommandFactory(Game &game, GameWorld &gameWorld):
m_lastCmd(CommandPtr(new Command)),
m_game(game),
m_gameWorld(gameWorld)
{
}

CommandPtr CommandFactory::parseCmd(const std::string &cmdStr)
{
    if(isEmptyString(cmdStr))
        return m_lastCmd;

    std::list<std::string> args = split(cmdStr);
    
    std::string cmdName = args.front();
    args.pop_front();
    
    CommandPtr cmd;
    if(cmdName == "q" || cmdName == "quit")
        cmd = CommandPtr(new QuitCommand(m_game));
    else if(cmdName == "s" || cmdName == "step")
    {
        int nbSteps = 1;
        if(!args.empty())
        {
            std::stringstream ss;
            ss << args.front();
            ss >> nbSteps;
        }
        
        if(nbSteps < 0)
            cmd = CommandPtr(new InvalidCommand);
        else
            cmd = CommandPtr(new StepCommand(m_gameWorld, nbSteps));
    }
    else if(cmdName == "h" || cmdName == "help")
        cmd = CommandPtr(new HelpCommand);
    else if(cmdName == "p" || cmdName == "print")
        cmd = CommandPtr(new PrintCommand(m_gameWorld));
    else if(cmdName == "a" || cmdName == "apply")
    {
        if(args.empty())
            cmd = CommandPtr(new InvalidCommand);
        else
        {
            EntityID id = 0;
        
            if(isInteger(args.front()))
            {
                std::stringstream ss;
                ss << args.front();
                ss >> id;
                args.pop_front();
            }
            std::string name = args.front();
            args.pop_front();
            cmd = CommandPtr(new ApplyCommand(m_gameWorld, id, name, concat(args)));
        }
    }
    else
        cmd = CommandPtr(new InvalidCommand);
        
    m_lastCmd = cmd;
    
    return cmd;
}


