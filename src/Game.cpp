#include "Game.hpp"
#include "Logger.hpp"


Game::Game():
m_isInteractive(false)
{
}

void Game::setOptions(Options& opt)
{
    std::list<Argument> args = opt.getArgs();

    for(auto it = args.begin();
        it != args.end();
        ++it)
    {
        Argument arg = *it;
        
        if(arg == "-i" || arg == "--interactive")
            m_isInteractive = true;
        else if(arg == "--enable-log")
            LOG.setEnabled(true);
        else if(arg == "--log-file")
            LOG.writeToFile(arg.getValue());
    }
}
    
void Game::run()
{
    if(m_isInteractive)
        runInteractiveMode();
    else
        runNonInteractiveMode();
}

void Game::runInteractiveMode()
{

}

void Game::runNonInteractiveMode()
{

}

