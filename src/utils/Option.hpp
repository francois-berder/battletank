#ifndef __OPTION_H__
#define __OPTION_H__

#include <list>
#include <string>

class Option
{
	public:

		Option(const std::string &name = std::string(),
				const std::string &value = std::string());

		static std::list<Option> parse(int argc, char **argv);
		static Option parseArg(char *arg);

		std::string getName() const;
		std::string getValue() const;

		bool operator==(const std::string &name);

	private:

		std::string m_name;
		std::string m_value;
};

#endif /* __OPTION_H__ */

