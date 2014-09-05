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

class Game
{
	public:

		Game();
        virtual ~Game();

		void setOptions(std::list<Option>& options);

		void run();

		void exit();

	private:

        void displayOptionsList();
    
		void executeFile();

		void proceedEvents();
        void proceedViewEvents();
        void proceedNetworkEvents();

		bool m_isInteractive;
		bool m_exit;
		GameWorld m_gameWorld;
		View m_view;
		std::string m_execFileName;
		std::ifstream m_replayFile;
        Server m_server;
        Client m_client;
        bool m_disableClient;
        bool m_runServer;
        std::list<NetworkEvent> m_events;
};

#endif /* __GAME_H__ */

