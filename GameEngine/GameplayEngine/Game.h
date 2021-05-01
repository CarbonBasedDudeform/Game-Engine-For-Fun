#pragma once
#include <string>
#include <memory>
#include "..\GraphicsEngine\Window.h"
#include "Scene.h"
#include "GameplayDataStructs.h"

namespace Gameplay
{

	class Game
	{
	public:
		Game();
		
		void SetTitle(std::string title);
		const std::string& GetTitle() const;
		void Initialise(std::string& title, size_t width, size_t height);
		void Run();
		void SetScene(std::shared_ptr<Gameplay::Scene>& scene);
	private:
		std::string _windowTitle;
		Graphics::Window _window;
		std::shared_ptr<Gameplay::Scene> _curScene;

		GameClock _clock;
		GameTimeDelta _lastUpdateTime;
	};
}
