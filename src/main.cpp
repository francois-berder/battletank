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
        LOG << "ERROR ! " << e.what() << "\n";
    }
    
    Game game;
    game.setOptions(opt);
    game.run();
    
    return 0;
}
