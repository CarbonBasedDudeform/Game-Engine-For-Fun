#include "Window.h"

namespace Graphics 
{
	void Window::Create(std::string & title, size_t height, size_t width)
	{
		ZeroMemory(&_window, sizeof(_window));
		_window.cbSize = sizeof(WNDCLASSEX);
		_window.style = CS_CLASSDC;
		_window.lpfnWndProc = WndProc;
		_window.hInstance = _instance;
		_window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		_window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		_window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		_window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		_window.lpszClassName = title.c_str();
		RegisterClassEx(&_window);
		_windowHandle = CreateWindow(title.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, width, height, nullptr, nullptr, _instance, nullptr);
		ShowWindow(_windowHandle, SW_SHOW);
		UpdateWindow(_windowHandle);
	}

	LRESULT Window::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	Window::Window(std::string& title, size_t height, size_t width)
	{
		Create(title, height, width);
	}


	Window::~Window()
	{
	}
}