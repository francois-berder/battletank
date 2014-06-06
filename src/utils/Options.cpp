#include <regex>

#include "Options.hpp"
#include "Logger.hpp"

Argument::Argument(const std::string &name, const std::string &value):
m_name(name),
m_value(value)
{
}

std::string Argument::getName()
{
    return m_name;
}

std::string Argument::getValue()
{
    return m_value;
}
        
        
Options::Options():
m_args()
{
}
        
void Options::parse(int argc, char **argv)
{
    m_args.clear();

    if(argc < 2)
        return;
    if(argv == NULL)
        return;
        
    for(int i = 1; i < argc; ++i)
        parseArg(argv[i]);
}

std::list<Argument> Options::getArgs()
{
    return m_args;
}

void Options::parseArg(char *arg)
{
    std::regex r("--?[a-zA-Z]+(=[a-zA-Z0-9]+)?");
    std::cmatch m;
    if(!std::regex_match(arg, m, r))    
    {
        std::stringstream msg;
        msg << "Malformed argument: " << arg << ".\n";    
        throw new std::runtime_error(msg.str());
    }
    std::string name, value;
    
    std::string s(arg);
    std::size_t i = s.find("=");
    if(i != std::string::npos)
    {
        name = s.substr(0, i);
        value = s.substr(i+1, s.size());
    }
    else
        name = s;
        
    m_args.push_back(Argument(name, value));
}

