#include <iostream>
#include <stdio.h>
#include "c_game.h"
#include "c_server.h"

int main(){

	c_Game game;
	c_Server server;

	server.Start(5555);
	game.Launch();

	game.JoinThread();
	server.JoinThread();
}