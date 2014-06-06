#include "Logger.hpp"

Logger::Logger():
m_enabled(true),
m_file()
{
}

Logger& Logger::instance()
{
    static Logger l;
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
