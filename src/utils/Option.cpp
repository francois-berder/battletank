#include <stdexcept>
#include <regex>

#include "Option.hpp"
#include "Logger.hpp"

Option::Option(const std::string &name, const std::string &value) :
		m_name(name), m_value(value)
{
}

bool Option::operator==(const std::string &name)
{
	return m_name == name;
}

std::string Option::getName() const
{
	return m_name;
}

std::string Option::getValue() const
{
	return m_value;
}

std::list<Option> Option::parse(std::list<std::string> args)
{
    std::list<Option> options;
    for(auto& arg : args)
        options.push_back(Option::parseArg(arg.c_str()));
    return options;
}

std::list<Option> Option::parse(int argc, char **argv)
{
	if(argv == NULL)
		throw std::runtime_error(
				"Could not parse options: null argument list given.");

	std::list<Option> options;

	if(argc < 2)
		return options;

	for(int i = 1; i < argc; ++i)
		options.push_back(Option::parseArg(argv[i]));

	return options;
}

Option Option::parseArg(const std::string &arg)
{
    return parseArg(const_cast<char*>(arg.c_str()));
}

Option Option::parseArg(char *arg)
{
	std::regex r("--?[a-zA-Z-]+(=[a-zA-Z0-9]+)?");
	std::cmatch m;
	if(!std::regex_match(arg, m, r))
	{
		std::stringstream msg;
		msg << "Malformed Option: " << arg;
		throw std::runtime_error(msg.str());
	}
	std::string name, value;

	std::string s(arg);
	std::size_t i = s.find("=");
	if(i != std::string::npos)
	{
		name = s.substr(0, i);
		value = s.substr(i + 1, s.size());
	}
	else
		name = s;

	return Option(name, value);
}

