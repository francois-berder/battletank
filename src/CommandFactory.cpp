#include <list>
#include <sstream>

#include "CommandFactory.hpp"
#include "Utils.hpp"

CommandFactory::CommandFactory(Game &game, GameWorld &gameWorld) :
		m_lastCmd(""), m_game(game), m_gameWorld(gameWorld)
{
}

CommandPtr CommandFactory::getCmd(const std::string &cmdStr)
{
	std::list<std::string> args;

	if(isEmpty(cmdStr))
        return parseCmd(m_game, m_gameWorld, m_lastCmd);

    m_lastCmd = cmdStr;
    return parseCmd(m_game, m_gameWorld, cmdStr);
}

CommandPtr CommandFactory::parseCmd(Game &game, GameWorld &gameWorld, const std::string &cmdStr)
{
	std::list<std::string> args;

	args = split(cmdStr, ' ');

	std::string cmdName = args.front();
	args.pop_front();

	CommandPtr cmd;
	if(cmdName == "q" || cmdName == "quit")
		cmd = CommandPtr(new QuitCommand(game));
	else if(cmdName == "s" || cmdName == "step")
	{
		int nbSteps = 1;
		if(!args.empty())
		{
			std::stringstream ss;
			ss << args.front();
			ss >> nbSteps;
		}

		if(nbSteps < 0)
			cmd = CommandPtr(new InvalidCommand);
		else
			cmd = CommandPtr(new StepCommand(gameWorld, nbSteps));
	}
	else if(cmdName == "h" || cmdName == "help")
		cmd = CommandPtr(new HelpCommand);
	else if(cmdName == "p" || cmdName == "print")
	{
		EntityID id = 0;
		if(!args.empty() && isInteger(args.front()))
		{
			std::stringstream ss;
			ss << args.front();
			ss >> id;
			args.pop_front();
		}
		cmd = CommandPtr(new PrintCommand(gameWorld, id));
	}
	else if(cmdName == "a" || cmdName == "apply")
	{
		if(args.empty())
			cmd = CommandPtr(new InvalidCommand);
		else
		{
			EntityID id = 0;

			if(isInteger(args.front()))
			{
				std::stringstream ss;
				ss << args.front();
				ss >> id;
				args.pop_front();
			}
			std::string name = args.front();
			args.pop_front();
			cmd = CommandPtr(new ApplyCommand(gameWorld, id, name, args));
		}
	}
	else
		cmd = CommandPtr(new InvalidCommand);

	return cmd;
}

