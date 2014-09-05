#include <stdexcept>

#include "Option.hpp"
#include "Logger.hpp"
#include "Game.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <unistd.h>

int main(int argc, char **argv)
{
	std::list<Option> options;
	try
	{
		options = Option::parse(argc, argv);
	}
	catch (std::exception &e)
	{
		Logger::error() << e.what() << "\n";
		Logger::instance() << "Abort.\n";
		return -1;
	}

	Game game;
	game.setOptions(options);
	game.run();
    
	return 0;
}
