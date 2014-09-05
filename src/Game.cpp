#include <stdexcept>
#include <sstream>

#include "Game.hpp"
#include "Logger.hpp"
#include "CommandFactory.hpp"
#include "Utils.hpp"


Game::Game() :
m_isInteractive(false),
m_exit(false),
m_gameWorld(),
m_view(),
m_execFileName(),
m_replayFile(),
m_server(),
m_client(),
m_disableClient(false)
{
}

Game::~Game()
{
    m_client.disconnect();
    m_server.stop();
}

void Game::setOptions(std::list<Option>& options)
{
	for(Option opt :options)
	{
		if(opt == "-i" || opt == "--interactive")
			m_isInteractive = true;
		else if(opt == "-h" || opt == "--help")
		{
		    displayOptionsList();
		    exit();
		}
		else if(opt == "--enable-log")
			Logger::instance().setEnabled(true);
		else if(opt == "--disable-log")
			Logger::instance().setEnabled(false);
		else if(opt == "--log-file")
			Logger::instance().writeToFile(opt.getValue());
		else if(opt == "--disable-input")
		    m_view.disableUserInput();
		else if(opt == "-x")
			m_execFileName = opt.getValue();
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
        else if(opt == "--run-server")
            m_server.startAcceptingClients();
        else if(opt == "--disable-client")
            m_disableClient = true;
	}
}

void Game::run()
{
    try
    {
        if(!m_execFileName.empty())
            executeFile();
    }
    catch(std::exception &e)
    {
        Logger::error() << "Error while trying to execute a file. Reason: " << e.what() << ".\n";
        exit();
    }
    
    try
    {
        if(!m_disableClient)
            m_client.connect("localhost");
    }
    catch(std::exception &e)
    {
        Logger::error() << "Error while trying to connect to server. Reason: " << e.what() << ".\n";
        exit();
    }
    m_server.stopAcceptingClients();

    m_server.initWorld();
    std::list<std::string> initCmds = m_client.getWorld();
    CommandFactory initCmdFactory(*this, m_gameWorld);
    for(auto &cmdStr : initCmds) 
    {
	    CommandPtr cmd = initCmdFactory.parseCmd(cmdStr);
        cmd->execute();
    }

    m_server.start();

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

void Game::displayOptionsList()
{
    std::cout << "[-i | --interactive]\t\tEnable interactive mode\n";
    std::cout << "[--enable-log | --disable-log]\tEnable/disable logging\n";
    std::cout << "[--log-file]\t\t\tLog will be saved in specified file\n";
    std::cout << "[-x]\t\t\t\tExecute commands from specified file\n";
    std::cout << "[-s | --save]\t\t\tSave all changes made to simulation in given file\n";
    std::cout << "[-r | --replay]\t\t\tReplay all commands from specified file\n";
}

void Game::executeFile()
{
	std::ifstream file(m_execFileName.c_str());
	if(!file)
	{
        std::stringstream ss;
        ss << "could not open file " << m_execFileName;
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
    proceedViewEvents();
    proceedNetworkEvents();
}

// Forward events from view to client
void Game::proceedViewEvents()
{
    Event viewEvent;
    while(m_view.pollEvent(viewEvent))
    {
        NetworkEvent event(m_client.getID(), m_gameWorld.getCurrentStep(), viewEvent);
        m_client.pushEvent(event);
    }    
}

void Game::proceedNetworkEvents()
{
	CommandFactory cmdFactory(*this, m_gameWorld);
	CommandPtr cmd;
    NetworkEvent netEvent;
	while(m_client.pollEvent(netEvent))
	{
        std::stringstream cmdStr;
		switch (netEvent.type)
		{
			case EventType::Quit :
				exit();
				break;
			case EventType::Left :
	    		cmd = cmdFactory.parseCmd("a 1 move left");
                if(!m_disableClient)
    		        cmd->execute();
			    break;
		    case EventType::Right :
	    		cmd = cmdFactory.parseCmd("a 1 move right");
                if(!m_disableClient)
    		        cmd->execute();
		        break;
	        case EventType::Up :
	    		cmd = cmdFactory.parseCmd("a 1 move down");
                if(!m_disableClient)
    		        cmd->execute();
	            break;
            case EventType::Down :
	    		cmd = cmdFactory.parseCmd("a 1 move up");
                if(!m_disableClient)
    		        cmd->execute();
                break;
            case EventType::Mouse :
                cmdStr << "a 1 dir ";
                cmdStr << netEvent.x << " " << netEvent.y;
	    		cmd = cmdFactory.parseCmd(cmdStr.str());
		        cmd->execute();
                break;
			default :
				throw std::runtime_error("Could not proceed unknown event.");
		}
	}
}

