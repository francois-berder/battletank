#include <sstream>
#include "Utils.hpp"

bool isEmpty(const std::string &str)
{
	if(str.empty())
		return true;

	for(unsigned int i = 0; i < str.size(); ++i)
		if(str.at(i) != ' ')
			return false;

	return true;
}

bool isInteger(const std::string &str)
{
	for(unsigned int i = 0; i < str.size(); ++i)
		if(!('0' <= str.at(i) && str.at(i) <= '9'))
			return false;
	return true;
}

int toInteger(const std::string &str)
{
	std::stringstream ss;
	ss << str;
	int val;
	ss >> val;
	return val;
}

float toFloat(const std::string &str)
{
	std::stringstream ss;
	ss << str;
	float val;
	ss >> val;
	return val;
}

std::list<std::string> split(const std::string &str, const char sep)
{
	std::list<std::string> elems;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, sep))
		elems.push_back(item);

	return elems;
}

std::string concat(std::list<std::string> &elems, const char sep)
{
	std::stringstream ss;
	for(auto it = elems.begin(); it != elems.end(); ++it)
	{
		ss << *it;
		if(std::next(it) != elems.end())
			ss << sep;
	}

	return ss.str();
}

