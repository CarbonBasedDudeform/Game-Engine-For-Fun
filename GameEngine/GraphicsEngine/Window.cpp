#include "Window.h"
#include "DX11Renderer.h"

namespace Graphics
{
	void Window::Create(std::string & title, size_t height, size_t width)
	{	
		WNDCLASSEX window;
		ZeroMemory(&window, sizeof(window));
		window.cbSize = sizeof(WNDCLASSEX);
		window.style = CS_CLASSDC;
		window.lpfnWndProc = WndProc;
		window.hInstance = _instance;
		window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		window.lpszClassName = title.c_str();

		RegisterClassEx(&window);
		_windowHandle = CreateWindow(title.c_str(), title.c_str(), WS_BORDER, 0, 0, width, height, nullptr, nullptr, _instance, nullptr);

		ShowWindow(_windowHandle, SW_SHOW);
		UpdateWindow(_windowHandle);		
		_renderer->CreateContext(height, width, _windowHandle);
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

	Window::Window(std::string& title, size_t height, size_t width, RendererTypes renderType) :
		_instance(0),
		_windowHandle(nullptr)
	{
		switch (renderType)
		{
		case RendererTypes::DX11:
			_renderer = std::make_unique<DX11Renderer>();
		}
		Create(title, height, width);
	}

	void Window::Loop() {
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				_updateFunc();
				_renderer->Render();
			}
		}
	}


	Window::~Window()
	{
	}

	void Window::Update(Models const& current_scene_models, const UpdateFunc&& updateFunc)
	{
		_updateFunc = std::move(updateFunc);
		_renderer->SetModelsToRender(current_scene_models);
	}
}