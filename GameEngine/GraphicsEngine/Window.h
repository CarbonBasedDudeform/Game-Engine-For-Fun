#pragma once
#include <string>
#include <Windows.h>

namespace Graphics
{
	class Window
	{
	public:
		Window(std::string& title, size_t height, size_t width);
		virtual ~Window();
	private:
		void Create(std::string& title, size_t height, size_t width);
		WNDCLASSEX _window;
		HINSTANCE _instance;
		HWND _windowHandle;
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
	};
}