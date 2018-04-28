#pragma once
#include <string>
#include <memory>
#include "..\GraphicsEngine\Window.h"
#include "Scene.h"

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
		void SetScene(std::unique_ptr<Gameplay::Scene> scene);
	private:
		std::string _windowTitle;
		std::unique_ptr<Graphics::Window> _window;
		std::unique_ptr<Gameplay::Scene> _curScene;
	};
}
