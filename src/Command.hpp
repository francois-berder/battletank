#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <memory>

#include "Game.hpp"
#include "GameWorld.hpp"


class Command
{
    public :
        
        virtual ~Command() = default;
        
        virtual void execute(Game &game, GameWorld &gameWorld);
};

typedef std::shared_ptr<Command> CommandPtr;

class InvalidCommand : public Command
{
    public :
    
        virtual void execute(Game &game, GameWorld &gameWorld);
};

class QuitCommand : public Command
{
    public :
    
        virtual void execute(Game &game, GameWorld &gameWorld);
};

class StepCommand : public Command
{
    public :

        StepCommand(const unsigned int nbSteps = 1);
    
        virtual void execute(Game &game, GameWorld &gameWorld);
        
    private :
    
        unsigned int m_nbSteps;
};

class HelpCommand : public Command
{
    public :
    
        virtual void execute(Game &game, GameWorld &gameWorld);
};

class PrintCommand : public Command
{
    public :
    
        virtual void execute(Game &game, GameWorld &gameWorld);
};

#endif /* __COMMAND_H__ */

