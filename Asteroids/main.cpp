#include <iostream>
#include <stdio.h>
#include "c_game.h"
#include "c_server.h"

int main(int argc, char *argv[]){

	c_Game game;
	c_Server server;
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			std::string arg = argv[i];
			if (arg == "client")
			{
				game.Launch();
				game.JoinThread();
			}
		}
	}
	else
	{
		server.Start(5555);
		game.Launch();

		game.JoinThread();
		server.JoinThread();
	}
}