#ifndef __COMMAND_FACTORY_H__
#define __COMMAND_FACTORY_H__

#include <string>

#include "Command.hpp"

class CommandFactory
{
	public:

		CommandFactory(Game &game, GameWorld &m_gameWorld);

        static CommandPtr parseCmd(Game &game, GameWorld &gameWorld, const std::string &cmdStr);

        CommandPtr getCmd(const std::string &cmdStr);

	private:

		std::string m_lastCmd;
		Game &m_game;
		GameWorld &m_gameWorld;
};

#endif /* __COMMAND_FACTORY_H__ */

