#include <sstream>

#include "Change.hpp"


Change::Change(const EntityID id, const std::string &name,
		const std::list<std::string> &args) :
		m_id(id), m_name(name), m_args(args)
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

std::list<std::string> Change::getArgs() const
{
	return m_args;
}

std::string Change::toString() const
{
    std::stringstream ss;
    ss << m_id << ' ' << m_name;
    for(std::string arg : m_args)
        ss << ' ' << arg;
    return ss.str();
}

// str =<id> <name> <arg0> <arg1> ...
// each element is separated by a blank.
Change Change::fromString(const std::string &str)
{
    std::stringstream ss;
    EntityID id;
    std::string name;
    std::list<std::string> args;
    char sep;
    ss << str;
    ss >> id >> sep >> name;
    std::string arg;
    while(!ss.eof())
    {
        ss >> sep >> arg;
        args.push_back(arg);
    }
    return Change(id, name, args);
}
