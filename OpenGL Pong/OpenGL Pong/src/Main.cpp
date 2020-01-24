#include <cstdlib>
#include <stdexcept>

#include "framework/utility/Logger.h"
#include "Pong.h"

int main(int argc, char* argv[])
{
	try
	{
		Pong game;
		game.Play();
	}
	catch (const std::exception& error)
	{
		Logger::Log(error.what());

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}