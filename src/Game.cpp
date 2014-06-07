#include <stdexcept>

#include "Game.hpp"
#include "Logger.hpp"
#include "CommandFactory.hpp"


Game::Game():
m_isInteractive(false),
m_exit(false),
m_gameWorld()
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

void Game::exit()
{
    m_exit = true;
}

void Game::runInteractiveMode()
{   
    CommandFactory cmdFactory;
    while(!m_gameWorld.isFinished() && !m_exit)
    {
        std::cout << "> ";
        std::string cmdStr;
        std::getline(std::cin, cmdStr);

        CommandPtr cmd = cmdFactory.parseCmd(cmdStr);
        cmd->execute(*this, m_gameWorld);
    }
}

void Game::runNonInteractiveMode()
{

}

