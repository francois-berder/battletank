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
m_execFile(*this, m_gameWorld),
m_replayFile(*this, m_gameWorld),
m_server(),
m_client(),
m_disableClient(false),
m_runServer(false),
m_events(),
m_serverHostname("localhost")
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
			m_execFile.open(opt.getValue());
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
        }
        else if(opt == "--run-server")
        {
            m_runServer = true;
            m_server.startAcceptingClients();
        }
        else if(opt == "--disable-network-client")
            m_disableClient = true;
        else if(opt == "--server-address")
            m_serverHostname = opt.getValue();
	}
}

void Game::init()
{
    try
    {
        m_execFile.execute();
    }
    catch(std::exception &e)
    {
        Logger::error() << "Error while trying to execute a file. Reason: " << e.what() << ".\n";
        exit();
    }
    
    try
    {
        if(!m_disableClient)
            m_client.connect(m_serverHostname);
    }
    catch(std::exception &e)
    {
        Logger::error() << "Error while trying to connect to server. Reason: " << e.what() << ".\n";
        exit();
    }

    if(m_runServer)
    {
        m_server.stopAcceptingClients();
        m_server.initWorld();
    }
    if(!m_disableClient)
    {
        std::list<std::string> initCmds = m_client.getWorld();
        for(auto &cmdStr : initCmds) 
        {
	        CommandPtr cmd = CommandFactory::parseCmd(*this, m_gameWorld, cmdStr);
            cmd->execute();
        }
    }

    if(m_runServer)
        m_server.start();

}

void Game::loop()
{
    CommandFactory cmdFactory(*this, m_gameWorld);
	while(!m_gameWorld.isFinished() && !m_exit)
	{
        m_replayFile.execute(m_gameWorld.getCurrentStep());

    	if(m_isInteractive)
	    {
		    std::cout << "> ";
		    std::string cmdStr;
		    std::getline(std::cin, cmdStr);
    		CommandPtr cmd = cmdFactory.getCmd(cmdStr);
	        cmd->execute();
        }
        else
    		m_gameWorld.step();

		m_view.update(m_gameWorld.print());
		proceedEvents();
	}
}

void Game::run()
{
    init();
    loop();
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

void Game::proceedEvents()
{
    proceedViewEvents();
    proceedNetworkEvents();
}

// Forward events from view to client
// If client is disabled, add it to the events list
void Game::proceedViewEvents()
{
    Event viewEvent;
    while(m_view.pollEvent(viewEvent))
    {
        unsigned int myID = 1;
        if(!m_disableClient)
            myID = m_client.getID();
        NetworkEvent event(myID, m_gameWorld.getCurrentStep(), viewEvent);
        if(m_disableClient)
            m_events.push_back(event);
        else
            m_client.pushEvent(event);
    }
}

void Game::proceedNetworkEvents()
{
    if(!m_disableClient)
    {
        NetworkEvent netEvent;
	    while(m_client.pollEvent(netEvent))
            m_events.push_back(netEvent); // FIXME: ensure order of events according to stepID
    }
    const unsigned int currentStep = m_gameWorld.getCurrentStep();

    while(m_events.front().stepID <= currentStep)
    {
        NetworkEvent e = m_events.front();
        std::stringstream cmdStr;
        cmdStr << "a " << e.clientID;
		switch (e.type)
		{
			case EventType::Quit :
				exit();
				return;
			case EventType::Left :
                cmdStr << " move left";
			    break;
		    case EventType::Right :
                cmdStr << " move right";
		        break;
	        case EventType::Up :
                cmdStr << " move up";
	            break;
            case EventType::Down :
                cmdStr << " move down";
                break;
            case EventType::Mouse :
                cmdStr << " dir " << e.x << " " << e.y;
                break;
            case EventType::Shoot :
                cmdStr << " shoot";
                break;
			default :
				throw std::runtime_error("Could not proceed unknown event.");
		}
		CommandPtr cmd = CommandFactory::parseCmd(*this, m_gameWorld, cmdStr.str());
        cmd->execute();
        m_events.erase(m_events.begin());
	}
}

