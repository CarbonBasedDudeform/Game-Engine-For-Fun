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
	auto game = Gameplay::Game{};
	game.Initialise(std::string("Demo App"), 1080, 1920);

	std::shared_ptr<Gameplay::Scene> scene = std::make_unique<TestScene>();
	//scene->AddModel(std::filesystem::current_path() / "Models" / "Sphere" / "sphere.obj");
	//scene->AddModel(std::filesystem::current_path() / "Models" / "Teapot" / "teapot.obj");
	//scene->AddModel(std::filesystem::current_path() / "Models" / "Cube" / "cube.obj");
	//scene->AddModel(std::filesystem::current_path() / "Models" / "Dragon" / "dragon.obj");
	//scene->AddModel(std::filesystem::current_path() / "Models" / "Sponza" / "sponza.obj");
	scene->AddModel(std::filesystem::current_path() / "Models" / "Sponza" / "mini_sponza.obj");
	//scene->AddModel(std::filesystem::current_path() / "Models" / "Gallery" / "gallery.obj");
	//scene->AddModel(std::filesystem::current_path() / "Models" / "Sibenik" / "sibenik.obj");
	//scene->AddModel(std::filesystem::current_path() / "Models" / "MultiModelTest" / "multimodeltest.obj");

	game.SetScene(scene); 
	game.Run();
}