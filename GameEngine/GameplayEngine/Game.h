#pragma once
#include <string>
#include <memory>
#include "..\GraphicsEngine\Window.h"

namespace Gameplay
{
	class Game
	{
	public:
		Game();
		~Game();
		void SetTitle(std::string title);
		const std::string& GetTitle() const;
		void Initialise(std::string& title, size_t width, size_t height);
		void Run();
	private:
		std::string _windowTitle;
		std::unique_ptr<Graphics::Window> _window;
	};
}
