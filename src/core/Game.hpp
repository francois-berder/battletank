#ifndef __GAME_H__
#define __GAME_H__

#include <fstream>
#include <queue>
#include <list>

#include "Option.hpp"
#include "GameWorld.hpp"
#include "View.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "CommandFile.hpp"

class Game
{
	public:

        Game();
        virtual ~Game();

		void setOptions(std::list<Option>& options);
        void init(sf::WindowHandle handle);

        void update();

		void exit();

	private:

        void displayOptionsList();
    
		void proceedEvents();
        void proceedViewEvents();
        void proceedNetworkEvents();

		bool m_isInteractive;
		bool m_exit;
		GameWorld m_gameWorld;
		View m_view;
        CommandFile m_execFile;
        CommandFile m_replayFile;
        Server m_server;
        Client m_client;
        bool m_disableClient;
        std::list<NetworkEvent> m_events;
        std::string m_serverHostname;
};

#endif /* __GAME_H__ */

