#include <stdexcept>
#include <sstream>

#include "Game.hpp"
#include "Logger.hpp"
#include "CommandFactory.hpp"
#include "Utils.hpp"


Game::Game() :
		m_isInteractive(false), m_exit(false), m_gameWorld(), m_view(*this), m_events(), m_execFile(), m_replayFile()
{
}

void Game::setOptions(std::list<Option>& options)
{
	for(Option opt :options)
	{
		if(opt == "-i" || opt == "--interactive")
			m_isInteractive = true;
		else if(opt == "--enable-log")
			LOG.setEnabled(true);
		else if(opt == "--log-file")
			LOG.writeToFile(opt.getValue());
		else if(opt == "-x")
			m_execFile = opt.getValue();
		else if(opt == "-s" || opt == "--save")
		{
		    try
		    {
			    m_gameWorld.saveToFile(opt.getValue());
            }
            catch(std::exception &e)
            {
                Logger::error() << e.what() << ". Save option ignored.\n";
            }
        }
        else if(opt == "-r" || opt == "--replay")
        {
            std::string replayFileName = opt.getValue().c_str();
            m_replayFile.open(replayFileName);
            if(!m_replayFile)
                Logger::error() << "Could not open file " << replayFileName << ". Replay option ignored.\n";
        }            
	}
}

void Game::run()
{
    try
    {
        if(!m_execFile.empty())
            executeFile();
    }
    catch(std::exception &e)
    {
        Logger::error() << "Error while trying to execute a file. Reason: " << e.what() << ".\n";
    }
        
	CommandFactory cmdFactory(*this, m_gameWorld);
	while(!m_gameWorld.isFinished() && !m_exit)
	{
	    if(m_replayFile.is_open())
	    {
	        while(true)
	        {
		        std::string cmdStr;
	            std::getline(m_replayFile, cmdStr, ' ');
	            unsigned int cmdStep = toUInteger(cmdStr);
	            
	            if(cmdStep == m_gameWorld.getCurrentStep())
	            {	        
	                std::getline(m_replayFile, cmdStr);
	                cmdStr = "a " + cmdStr;
	        	    CommandFactory cmdFactory2(*this, m_gameWorld);
        		    CommandPtr cmd = cmdFactory2.parseCmd(cmdStr);
	                cmd->execute();
	            }
	            else
	            {
	                m_replayFile.seekg(-cmdStr.size()-1, std::ios_base::cur);
	                break;
	            }
	        }
	    }
	    
    	if(m_isInteractive)
	    {
		    std::cout << "> ";
		    std::string cmdStr;
		    std::getline(std::cin, cmdStr);
    		CommandPtr cmd = cmdFactory.parseCmd(cmdStr);
	        cmd->execute();
        }
        else
    		m_gameWorld.step();
	
		m_view.update(m_gameWorld.print());
		proceedEvents();
	}
}

void Game::exit()
{
	m_exit = true;
}

void Game::pushEvent(const Event event)
{
	m_events.push(event);
}

void Game::executeFile()
{
	std::ifstream file(m_execFile.c_str());
	if(!file)
	{
        std::stringstream ss;
        ss << "could not open file " << m_execFile;
	    throw std::runtime_error(ss.str());
	}

	CommandFactory cmdFactory(*this, m_gameWorld);
	std::string cmdStr;
	while (std::getline(file, cmdStr))
	{
		CommandPtr cmd = cmdFactory.parseCmd(cmdStr);
		cmd->execute();
	}

	file.close();
}

void Game::proceedEvents()
{
	CommandFactory cmdFactory(*this, m_gameWorld);
	CommandPtr cmd;
	while (!m_events.empty())
	{
        std::stringstream cmdStr;
	    Event e = m_events.front();
		switch (e.type)
		{
			case EventType::Quit :
				exit();
				break;
			case EventType::Left :
	    		cmd = cmdFactory.parseCmd("a 1 move left");
		        cmd->execute();
			    break;
		    case EventType::Right :
	    		cmd = cmdFactory.parseCmd("a 1 move right");
		        cmd->execute();
		        break;
	        case EventType::Up :
	    		cmd = cmdFactory.parseCmd("a 1 move down");
		        cmd->execute();
	            break;
            case EventType::Down :
	    		cmd = cmdFactory.parseCmd("a 1 move up");
		        cmd->execute();
                break;
            case EventType::Mouse :
                cmdStr << "a 1 dir ";
                cmdStr << e.x << " " << e.y;
	    		cmd = cmdFactory.parseCmd(cmdStr.str());
		        cmd->execute();
                break;
			default :
				throw std::runtime_error("Could not proceed unknown event.");
		}
		m_events.pop();
	}
}

