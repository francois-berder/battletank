#include <stdexcept>

#include "Options.hpp"
#include "Logger.hpp"
#include "Game.hpp"


int main(int argc, char **argv)
{
    Options opt;
    try
    {
        opt.parse(argc, argv);
    }
    catch(std::exception &e)
    {
        LOG << e.what() << "\n";
        LOG << "Abort.\n";
        return -1;
    }
    
    Game game;
    game.setOptions(opt);
    game.run();
    
    return 0;
}
