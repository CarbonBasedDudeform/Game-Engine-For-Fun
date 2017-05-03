#include "Game.h"


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
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}