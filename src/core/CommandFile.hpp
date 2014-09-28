#ifndef __COMMAND_FILE_HPP__
#define __COMMAND_FILE_HPP__

#include <string>
#include <fstream>

class Game;
class GameWorld;


// This class manages a file which contains a list of command
class CommandFile
{
    public :

        CommandFile(Game &game, GameWorld &gameWorld);
        virtual ~CommandFile();

        void open(const std::string &fileName);
        void execute();
        void execute(unsigned int untilStepID);

    private :

        Game &m_game;
        GameWorld &m_gameWorld;
        std::ifstream m_file;
};

#endif /* __COMMAND_FILE_HPP__ */

