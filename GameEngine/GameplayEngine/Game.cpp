#include "Game.h"
#include <thread>
#include <functional>

namespace Gameplay
{
	Game::Game()
	{
	}


	Game::~Game()
	{
	}

	void Game::SetTitle(std::string title)
	{
		_windowTitle = title;
	}

	const std::string & Game::GetTitle() const
	{
		return _windowTitle;
	}

	void Game::Initialise(std::string& title, size_t width, size_t height)
	{
		_window = std::make_unique<Graphics::Window>(title, width, height);
	}

	void Game::Run()
	{
		_window->Update([=] { 
			_curScene->Update(); 
		});
	}
	void Game::SetScene(std::unique_ptr<Gameplay::Scene> scene)
	{
		_curScene = std::move(scene);
	}
}