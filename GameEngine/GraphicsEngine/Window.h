#pragma once
#include <string>
#include <Windows.h>
#include <functional>
#include <memory>
#include "IRenderer.h"


namespace Graphics
{
	class Window
	{
	public:
		Window(std::string& title, size_t height, size_t width, RendererTypes renderType);
		virtual ~Window();
		void Update(const std::function<void()> gameLoop);
	private:
		void Create(std::string& title, size_t height, size_t width);
		WNDCLASSEX _window;
		HINSTANCE _instance;
		HWND _windowHandle;
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
		 
		std::unique_ptr<IRenderer> _renderer;
	};
}