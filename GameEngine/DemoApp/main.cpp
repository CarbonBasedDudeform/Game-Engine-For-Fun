#include <iostream>
#include <memory>
#include <Game.h>
void main()
{
	auto game = std::make_unique<Gameplay::Game>();
	game->Initialise(std::string("Demo App"), 600, 800);
	game->Run();
}