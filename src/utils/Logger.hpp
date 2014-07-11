#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
#include <sstream>

#define LOG Logger::instance()

class Logger
{
	public:

		Logger();

		static Logger& instance();

		template<typename T>
		Logger& operator<<(T msg)
		{
			if(!m_enabled)
				return *this;

			if(m_file.is_open())
			{
				std::stringstream ss;
				ss << msg;
				std::string s = ss.str();
				m_file.write(s.c_str(), s.size());
			}
			else
				std::cout << msg;

			return *this;
		}

		void setEnabled(const bool enabled);

		void writeToFile(const std::string& fileName);
		void writeToConsole();

	private:

		bool m_enabled;
		std::ofstream m_file;
};

#endif /* __LOGGER_H__ */

