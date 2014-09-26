#include <sstream>
#include <stdexcept>

#include "CommandFile.hpp"
#include "CommandFactory.hpp"
#include "Game.hpp"
#include "GameWorld.hpp"
#include "Utils.hpp"


CommandFile::CommandFile(Game &game, GameWorld &gameWorld):
m_game(game),
m_gameWorld(gameWorld),
m_file()
{
}

CommandFile::~CommandFile()
{
	m_file.close();
}

void CommandFile::open(const std::string &fileName)
{
	m_file.open(fileName.c_str());
	if(!m_file)
	{
        std::stringstream ss;
        ss << "could not open file " << fileName;
	    throw std::runtime_error(ss.str());
	}
}

void CommandFile::execute()
{
    if(!m_file.is_open())
        return;
    
	std::string cmdStr;
	while (std::getline(m_file, cmdStr))
	{
		CommandPtr cmd = CommandFactory::parseCmd(m_game, m_gameWorld, cmdStr);
		cmd->execute();
	}

    m_file.close();
}

void CommandFile::execute(unsigned int untilStepID)
{
    if(!m_file.is_open())
        return;

    while(true)
    {
        std::string cmdStr;
        std::getline(m_file, cmdStr, ' ');
        unsigned int cmdStep = toUInteger(cmdStr);
        
        if(cmdStep == untilStepID)
        {
            std::getline(m_file, cmdStr);
            cmdStr = "a " + cmdStr;
		    CommandPtr cmd = CommandFactory::parseCmd(m_game, m_gameWorld, cmdStr);
            cmd->execute();
        }
        else
        {
            m_file.seekg(-cmdStr.size()-1, std::ios_base::cur);
            break;
        }
    }
}
