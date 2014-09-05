#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>

class Logger
{
	public:

		Logger();

		static Logger& instance();

		static Logger& info();
		static Logger& warning();
		static Logger& error();
		
		template<typename T>
		Logger& operator<<(T msg)
		{
			if(!m_enabled)
				return *this;

			if(m_file.is_open())
			{
                std::lock_guard<std::mutex> lock(m_mutex);
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
        std::mutex m_mutex;
};

#endif /* __LOGGER_H__ */

