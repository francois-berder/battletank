#include <stdexcept>
#include <sstream>

#include "Game.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include "EntityParser.hpp"

Game::Game() :
m_gameWorld(),
m_view(),
m_server(),
m_client(),
m_events(),
m_worldInitialized(false)
{
}

Game::~Game()
{
    exit();
}

/*
 * Returns true, if game continues,
 * otherwise returns false (end of game).
 */
bool Game::update()
{
    // update state
    handleEvents();
    m_gameWorld.step();

    // draw state
    EntityParser parser(m_gameWorld.print());
    Array entities = parser.parse();
    m_view.update(entities);

    return !isGameFinished(entities);
}

void Game::initView(sf::WindowHandle handle)
{
    m_view.init(handle);
}

void Game::initWorld()
{
    std::list<std::string> initCmds = m_client.getWorld();
    for(auto &cmdStr : initCmds)
        m_gameWorld.applyChange(Change::fromString(cmdStr));

    m_worldInitialized = true;
}

void Game::exit()
{
    m_worldInitialized = false;
    m_client.disconnect();
    m_server.waitUntilAllClientsDisconnected();
    m_server.stop();
}

void Game::handleEvents()
{
    handleViewEvents();
    handleNetworkEvents();
}

// Forward events from view to client
void Game::handleViewEvents()
{
    Event viewEvent;
    while(m_view.pollEvent(viewEvent))
    {
        unsigned int myID = m_client.getID();
        NetworkEvent event(myID, m_gameWorld.getCurrentStep(), viewEvent);
        m_client.pushEvent(event);
    }
}

void Game::handleNetworkEvents()
{
    NetworkEvent netEvent;
    while(m_client.pollEvent(netEvent))
        m_events.push_back(netEvent); // FIXME: ensure order of events according to stepID

    while(!m_events.empty())
    {
        const NetworkEvent e = m_events.front();

        if(e.stepID > m_gameWorld.getCurrentStep())
            break;

        m_gameWorld.applyChange(e.toChange());
        m_events.erase(m_events.begin());
	}
}

// Check if the game is finished
// Either 0 or 1 tank alive left.
bool Game::isGameFinished(Array &entities)
{
    unsigned int nbTanksAlive = 0;

    for(unsigned int i = 0; i < entities.size(); ++i)
    {
        Map &entity = entities[i]->asMap();
        std::string type = entity["type"]->asData().getValue();
        if(type == "tank")
        {
            unsigned int health = toUInteger(entity["health"]->asData().getValue());
            if(health > 0)
                ++nbTanksAlive;
        }
    }

    return nbTanksAlive <= 1 && m_worldInitialized;
}

Server& Game::getServer()
{
    return m_server;
}

Client& Game::getClient()
{
    return m_client;
}
