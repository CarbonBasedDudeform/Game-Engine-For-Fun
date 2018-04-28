#include <iostream>
#include <memory>
#include <Game.h>
#include <Scene.h>

class TestScene : public Gameplay::Scene
{
	void Update() 
	{

	}
};

void main()
{
	auto game = std::make_unique<Gameplay::Game>();
	game->Initialise(std::string("Demo App"), 600, 800);

	std::unique_ptr<Gameplay::Scene> scene = std::make_unique<TestScene>();
	game->SetScene(std::move(scene));
	game->Run();
}