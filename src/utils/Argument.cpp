#include <sstream>

#include "Argument.hpp"

Argument::Argument(const std::string &str) :
		std::string(str)
{
}

std::list<Argument> Argument::parse(const std::string &arg)
{
	std::list<Argument> elems;
	std::stringstream ss(arg);
	Argument item;
	while (std::getline(ss, item, ' '))
		elems.push_back(item);

	return elems;
}

std::string Argument::concat(std::list<Argument> &args)
{
	std::stringstream ss;
	for(auto it = args.begin(); it != args.end(); ++it)
	{
		ss << *it;
		if(std::next(it) != args.end())
			ss << " ";
	}

	return ss.str();
}

bool Argument::isEmpty() const
{
	if(empty())
		return true;

	for(unsigned int i = 0; i < size(); ++i)
		if(at(i) != ' ')
			return false;

	return true;
}

bool Argument::isInteger() const
{
	for(unsigned int i = 0; i < size(); ++i)
		if(!('0' <= at(i) && at(i) <= '9'))
			return false;
	return true;
}

std::string Argument::toString() const
{
	return *this;
}

float Argument::toFloat() const
{
	std::stringstream ss;
	ss << toString();
	float val;
	ss >> val;
	return val;
}

