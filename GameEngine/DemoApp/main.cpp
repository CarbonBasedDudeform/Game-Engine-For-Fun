#include <iostream>
#include <memory>
#include <Game.h>
#include <Scene.h>

#include <filesystem>

class TestScene : public Gameplay::Scene
{
	void RenderFrame()
	{
		
	}

	void Update(Gameplay::GameTimeDelta timeDelta)
	{

	}
};

MAIN
{
	auto game = std::make_unique<Gameplay::Game>();
	game->Initialise(std::string("Demo App"), 600, 800);

	std::unique_ptr<Gameplay::Scene> scene = std::make_unique<TestScene>();
	scene->AddModel(std::filesystem::current_path() / "Models" / "Teapot" / "teapot.obj");
	//scene->AddModel(std::filesystem::current_path() / "Models" / "Cube" / "cube.obj");
	//scene->AddModel(std::filesystem::current_path() / "Models" / "Dragon" / "dragon.obj");
	game->SetScene(std::move(scene));
	game->Run();
}