#include "Change.hpp"

Change::Change(const EntityID id, const std::string &name, const std::list<Argument> &args):
m_id(id),
m_name(name),
m_args(args)
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

std::list<Argument> Change::getArgs() const
{
    return m_args;
}

