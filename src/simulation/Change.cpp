#include "Change.hpp"

Change::Change(const EntityID id, const std::string &name, const std::string &arg):
m_id(id),
m_name(name),
m_arg(arg)
{
}

EntityID Change::getTargetID() const
{
    return m_id;
}

std::string Change::getName() const
{   
    return m_name;
}

std::string Change::getArg() const
{
    return m_arg;
}

