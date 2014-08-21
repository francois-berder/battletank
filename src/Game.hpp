#ifndef __GAME_H__
#define __GAME_H__

#include <queue>

#include "Option.hpp"
#include "GameWorld.hpp"
#include "View.hpp"
#include "Event.hpp"

class Game
{
	public:

		Game();

		void setOptions(std::list<Option>& options);

		void run();

		void exit();

		void pushEvent(const Event event);

	private:

		void runInteractiveMode();
		void runNonInteractiveMode();

		void executeFile();

		void proceedEvents();

		bool m_isInteractive;
		bool m_exit;
		GameWorld m_gameWorld;
		View m_view;
		std::queue<Event> m_events;
		std::string m_execFile;
};

#endif /* __GAME_H__ */

