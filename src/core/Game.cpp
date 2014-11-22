#include <stdexcept>
#include <sstream>

#include "Game.hpp"
#include "Logger.hpp"
#include "CommandFactory.hpp"
#include "Utils.hpp"


Game::Game() :
m_gameWorld(),
m_view(),
m_server(),
m_client(),
m_events(),
m_serverHostname("localhost")
{
}

Game::~Game()
{
    exit();
}

void Game::update()
{
    proceedEvents();
    m_gameWorld.step();
    m_view.update(m_gameWorld.print());
}

void Game::initView(sf::WindowHandle handle)
{
    m_view.init(handle);
}

void Game::initWorld()
{
    std::list<std::string> initCmds = m_client.getWorld();
    for(auto &cmdStr : initCmds)
    {
        CommandPtr cmd = CommandFactory::parseCmd(*this, m_gameWorld, cmdStr);
        cmd->execute();
    }
}

void Game::exit()
{
    m_client.disconnect();
    m_server.waitUntilAllClientsDisconnected();
    m_server.stop();
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
        unsigned int myID = m_client.getID();
        NetworkEvent event(myID, m_gameWorld.getCurrentStep(), viewEvent);
        m_client.pushEvent(event);
    }
}

void Game::proceedNetworkEvents()
{
    NetworkEvent netEvent;
    while(m_client.pollEvent(netEvent))
        m_events.push_back(netEvent); // FIXME: ensure order of events according to stepID

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

// Check if the game is finished
// Either 0 or 1 tank alive left.
bool Game::isGameFinished()
{
    // TODO: implement function
    return false;
}


Server& Game::getServer()
{
    return m_server;
}

Client& Game::getClient()
{
    return m_client;
}
