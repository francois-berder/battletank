#ifndef __GAME_H__
#define __GAME_H__

#include <fstream>
#include <queue>
#include <list>

#include "GameWorld.hpp"
#include "View.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Game
{
	public:

        Game();
        virtual ~Game();

        void initView(sf::WindowHandle handle);
        void initWorld();

        void update();
        bool isGameFinished();
		void exit();

        Server& getServer();
        Client& getClient();

	private:
    
        void handleEvents();
        void handleViewEvents();
        void handleNetworkEvents();

		GameWorld m_gameWorld;
		View m_view;
        Server m_server;
        Client m_client;
        std::list<NetworkEvent> m_events;
};

#endif /* __GAME_H__ */

