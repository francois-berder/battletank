#include <stdexcept>

#include "Option.hpp"
#include "Logger.hpp"
#include "Game.hpp"


int main(int argc, char **argv)
{
    std::list<Option> options;
    try
    {
        options = Option::parse(argc, argv);
    }
    catch(std::exception &e)
    {
        LOG << e.what() << "\n";
        LOG << "Abort.\n";
        return -1;
    }
    
    Game game;
    game.setOptions(options);
    game.run();
    
    return 0;
}
