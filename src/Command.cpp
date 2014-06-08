#include <iostream>

#include "Command.hpp"


void Command::execute(Game &game, GameWorld &gameWorld)
{
}

void InvalidCommand::execute(Game &game, GameWorld &gameWorld)
{
    std::cout << "Invalid command." << std::endl;
}
    
void QuitCommand::execute(Game &game, GameWorld &gameWorld)
{
    std::cout << "Exiting." << std::endl;
    game.exit();
}

void StepCommand::execute(Game &game, GameWorld &gameWorld)
{
    gameWorld.step();
}

void HelpCommand::execute(Game &game, GameWorld &gameWorld)
{
    std::cout << "Command list:" << std::endl;
    std::cout << "step\tExecute one simulation step" << std::endl;
    std::cout << "print\tPrint gameworld state" << std::endl;
    std::cout << "help\tDisplay help" << std::endl;
    std::cout << "quit\tQuit game" << std::endl;    
}

void PrintCommand::execute(Game &game, GameWorld &gameWorld)
{
    std::cout << gameWorld.print() << std::endl;   
}

