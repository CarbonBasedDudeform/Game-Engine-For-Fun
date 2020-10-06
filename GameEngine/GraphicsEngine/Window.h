#pragma once

#pragma warning(push, 0) //disable warnings for external headers
#include <Windows.h>
#include <string>
#include <functional>
#include <memory>
#pragma warning(pop)


#include "IRenderer.h"
#include "FreeRoamProjectionCamera.h"
#include "Model.h"

#define MAIN int CALLBACK WinMain(__in  HINSTANCE hInstance,__in  HINSTANCE hPrevInstance,__in  LPSTR lpCmdLine,__in  int nCmdShow)

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
		Window(std::string& title, size_t height, size_t width, RendererTypes renderType);

		void Update(Models const& current_scene_models, UpdateFunc const&& updateFunc);
		void Loop();

	private:
		void Create(std::string const& title, size_t height, size_t width);
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
		void recenter_cursor();
		
	private:
		HINSTANCE instance_{};
		HWND window_handle_{};
		 
		std::unique_ptr<IRenderer> renderer_;
		UpdateFunc update_func_{};

		std::shared_ptr<PAL::InputManager> input_manager_{};
		Graphics::FreeRoamProjectionCamera camera_{0, 2.5f, -10.0f};
	};
}