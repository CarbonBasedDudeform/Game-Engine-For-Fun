#pragma once

#pragma warning(push, 0) //disable warnings for external headers
#include <Windows.h>

#include <string>
#include <functional>
#include <memory>
#pragma warning(pop)

#include "IRenderer.h"
#include "Model.h"

#define MAIN int CALLBACK WinMain(__in  HINSTANCE hInstance,__in  HINSTANCE hPrevInstance,__in  LPSTR lpCmdLine,__in  int nCmdShow)

namespace Graphics
{
	class Window
	{
	public:
		Window(std::string& title, size_t height, size_t width, RendererTypes renderType);
		virtual ~Window() = default;

		void Update(Models const& current_scene_models, const UpdateFunc&& updateFunc);
		void Loop();
	private:
		void Create(std::string& title, size_t height, size_t width);
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HINSTANCE instance_{};
		HWND window_handle_{};
		
		 
		std::unique_ptr<IRenderer> renderer_;
		UpdateFunc update_func_{};
	};
}