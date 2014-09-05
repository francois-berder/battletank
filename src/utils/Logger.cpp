#include "Logger.hpp"

Logger::Logger() :
m_enabled(true), 
m_file(), 
m_mutex()
{
}

Logger& Logger::instance()
{
	static Logger l;
	return l;
}

Logger& Logger::info()
{
    Logger &l = instance();
    l << "INFO: ";
    return l;
}

Logger& Logger::warning()
{
    Logger &l = instance();
    l << "WARN: ";
    return l;
}

Logger& Logger::error()
{
    Logger &l = instance();
    l << "ERROR: ";
    return l;
}
		
void Logger::setEnabled(const bool enabled)
{
	m_enabled = enabled;
}

void Logger::writeToFile(const std::string& fileName)
{
	m_file.open(fileName.c_str());
	if(!m_file)
	{
		std::cout << "Could not log to " << fileName << "." << std::endl;
		std::cout << "Fallback to console.";
	}
}

void Logger::writeToConsole()
{
	m_file.close();
}

