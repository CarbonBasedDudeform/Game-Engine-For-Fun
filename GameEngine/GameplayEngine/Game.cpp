#include "Game.h"
#include <thread>
#include <functional>

namespace Gameplay
{
	Game::Game()
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

	void Game::Initialise(const std::string& title, size_t width, size_t height)
	{
		_window = Graphics::Window(title, width, height, Graphics::RendererTypes::DX11);
	}

	void Game::Run()
	{
		_window.SetScene(_curScene->getModels(),
			[=] {
				auto currentTime = _clock.now();
				auto deltaTime = static_cast<GameTimeDelta>(currentTime - _lastUpdateTime);
				_curScene->Update(deltaTime);
				_lastUpdateTime = currentTime;
			});
		
		_window.Loop();
	}

	void Game::SetScene(std::shared_ptr<Gameplay::Scene>& scene)
	{
		_curScene = scene;
	}
}