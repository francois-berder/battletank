#include <list>
#include <sstream>

#include "CommandFactory.hpp"
#include "Utils.hpp"

CommandFactory::CommandFactory(Game &game, GameWorld &gameWorld) :
		m_lastCmd(""), m_game(game), m_gameWorld(gameWorld)
{
}

CommandPtr CommandFactory::parseCmd(const std::string &cmdStr)
{
	std::list<std::string> args;

	if(isEmpty(cmdStr))
		args = split(m_lastCmd, ' ');
	else
	{
		args = split(cmdStr, ' ');
		m_lastCmd = cmdStr;
	}

	std::string cmdName = args.front();
	args.pop_front();

	CommandPtr cmd;
	if(cmdName == "q" || cmdName == "quit")
		cmd = CommandPtr(new QuitCommand(m_game));
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
			cmd = CommandPtr(new StepCommand(m_gameWorld, nbSteps));
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
		cmd = CommandPtr(new PrintCommand(m_gameWorld, id));
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
			cmd = CommandPtr(new ApplyCommand(m_gameWorld, id, name, args));
		}
	}
	else
		cmd = CommandPtr(new InvalidCommand);

	return cmd;
}

