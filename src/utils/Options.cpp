#include <regex>

#include "Options.hpp"


Options::Options():
m_values()
{
}
        
void Options::parse(int argc, char **argv)
{
    m_values.clear();

    if(argc < 2)
        return;
    if(argv == NULL)
        return;
}

std::string Options::getValue(const std::string &name)
{
    std::map<std::string, std::string>::iterator it = m_values.find(name);
    if(it != m_values.end())
        return it->second;
    else
        return std::string();
}
