#include <iostream>

#include "Command.hpp"

void Command::execute()
{
}

void InvalidCommand::execute()
{
	std::cout << "Invalid command." << std::endl;
}

QuitCommand::QuitCommand(Game& game) :
		Command(), m_game(game)
{
}

void QuitCommand::execute()
{
	std::cout << "Exiting." << std::endl;
	m_game.exit();
}

StepCommand::StepCommand(GameWorld &gameWorld, const unsigned int nbSteps) :
		Command(), m_gameWorld(gameWorld), m_nbSteps(nbSteps)
{
}

void StepCommand::execute()
{
	while (m_nbSteps != 0)
	{
		m_gameWorld.step();
		--m_nbSteps;
	}
}

void HelpCommand::execute()
{
	std::cout << "Command list:" << std::endl;
	std::cout << "step\tExecute one simulation step" << std::endl;
	std::cout << "print\tPrint gameworld state" << std::endl;
	std::cout << "apply\tApply change to game" << std::endl;
	std::cout << "help\tDisplay help" << std::endl;
	std::cout << "quit\tQuit game" << std::endl;
}

PrintCommand::PrintCommand(GameWorld &gameWorld, EntityID id) :
		Command(), m_gameWorld(gameWorld), m_id(id)
{
}

void PrintCommand::execute()
{
	if(m_id == 0)
	{
		std::cout << "step " << m_gameWorld.getCurrentStep() << ":" << std::endl;
		std::cout << m_gameWorld.print() << std::endl;
	}
	else
	{
		try
		{
			std::cout << m_gameWorld.printEntity(m_id) << std::endl;
		}
		catch (std::exception &e)
		{
			std::cout << "Error: " << e.what() << std::endl;
		}
	}
}

ApplyCommand::ApplyCommand(GameWorld &gameWorld, const EntityID id,
		const std::string name, const std::list<Argument> &args) :
		Command(), m_gameWorld(gameWorld), m_change(id, name, args)
{
}

void ApplyCommand::execute()
{
	m_gameWorld.applyChange(m_change);
}

