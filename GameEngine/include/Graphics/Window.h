#pragma once

#include <string>
#include <functional>
#include <memory>

//#include "IRenderer.h"
//#include "FreeRoamProjectionCamera.h"
//#include "Model.h"
//#include "IUserInterfaceManager.h"

struct SDL_Window;

namespace PAL
{
	class InputManager;
}

namespace Graphics
{
	class Window final
	{
	public:
		Window() = default;
		Window(const std::string& title, size_t height, size_t width);
		~Window();

		//void set_scene(Models const& current_scene_models, UpdateFunc const&& updateFunc);
		void loop();

	private:
		void create(const std::string& title, size_t height, size_t width);
		
	private:
		static std::function<void(SDL_Window*)> sdl_deleter;
		std::unique_ptr<SDL_Window, decltype(sdl_deleter)> window {};
		//UpdateFunc update_func{};

		//std::unique_ptr<IRenderer> renderer{};
		//std::shared_ptr<UI::IUserInterfaceManager> user_interface{};
		//std::shared_ptr<PAL::InputManager> input_manager{};
		//Graphics::FreeRoamProjectionCamera camera{0, 2.5f, -10.0f};
	};
}