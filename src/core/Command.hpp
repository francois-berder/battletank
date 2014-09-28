#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <memory>
#include <list>

#include "Game.hpp"
#include "GameWorld.hpp"

class Command
{
	public:

		virtual ~Command() = default;

		virtual void execute();
};

typedef std::shared_ptr<Command> CommandPtr;

class InvalidCommand : public Command
{
	public:

		virtual void execute();
};

class QuitCommand : public Command
{
	public:

		QuitCommand(Game &game);

		virtual void execute();

	private:

		Game &m_game;
};

class StepCommand : public Command
{
	public:

		StepCommand(GameWorld &gameWorld, const unsigned int nbSteps = 1);

		virtual void execute();

	private:

		GameWorld &m_gameWorld;
		unsigned int m_nbSteps;
};

class HelpCommand : public Command
{
	public:

		virtual void execute();
};

class PrintCommand : public Command
{
	public:

		PrintCommand(GameWorld &gameWorld, EntityID id);

		virtual void execute();

	private:

		GameWorld &m_gameWorld;
		EntityID m_id;
};

class ApplyCommand : public Command
{
	public:

		ApplyCommand(GameWorld &gameWorld, const EntityID id,
				const std::string name, const std::list<std::string> &args);

		virtual void execute();

	private:

		GameWorld &m_gameWorld;
		Change m_change;
};

#endif /* __COMMAND_H__ */

