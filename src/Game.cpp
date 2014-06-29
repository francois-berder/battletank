#include <fstream>
#include <stdexcept>

#include "Game.hpp"
#include "Logger.hpp"
#include "CommandFactory.hpp"


Game::Game():
m_isInteractive(false),
m_exit(false),
m_gameWorld(),
m_view(*this),
m_events()
{
}

void Game::setOptions(std::list<Option>& options)
{
    for(auto it = options.begin();
        it != options.end();
        ++it)
    {
        Option opt = *it;
        
        if(opt == "-i" || opt == "--interactive")
            m_isInteractive = true;
        else if(opt == "--enable-log")
            LOG.setEnabled(true);
        else if(opt == "--log-file")
            LOG.writeToFile(opt.getValue());
        else if(opt == "-x")
            executeFile(opt.getValue());
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

void Game::pushEvent(const Event& event)
{
    m_events.push(event);
}

void Game::runInteractiveMode()
{   
    CommandFactory cmdFactory(*this, m_gameWorld);
    while(!m_gameWorld.isFinished() && !m_exit)
    {
        std::cout << "> ";
        std::string cmdStr;
        std::getline(std::cin, cmdStr);

        CommandPtr cmd = cmdFactory.parseCmd(cmdStr);
        cmd->execute();
        m_view.update(m_gameWorld.print());
        proceedEvents();
    }
}

void Game::runNonInteractiveMode()
{
    while(!m_gameWorld.isFinished() && !m_exit)
    {
        m_gameWorld.step();
        m_view.update(m_gameWorld.print());
        proceedEvents();
    }
}

void Game::executeFile(const std::string& path)
{
    std::ifstream file(path.c_str());
    if(!file)
    {
        Logger::instance() << "Could not execute file " << path << ".\n";
        return;
    }

    CommandFactory cmdFactory(*this, m_gameWorld);    
    std::string cmdStr;
    while(std::getline(file, cmdStr))
    {
        CommandPtr cmd = cmdFactory.parseCmd(cmdStr);
        cmd->execute();
    }
    
    file.close();
}

void Game::proceedEvents()
{
    while(!m_events.empty())
    {
        Event evt = m_events.front();
        switch(evt)
        {
            case Event::QuitEvent:
                exit();
                break;
            default:
                throw std::runtime_error("Could not proceed unknown event."); 
        }
        m_events.pop();
    }
}


