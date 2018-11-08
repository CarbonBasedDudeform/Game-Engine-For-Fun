#pragma once
#include <Windows.h>

#include <string>
#include <functional>
#include <memory>

#include "IRenderer.h"
#define MAIN int CALLBACK WinMain(__in  HINSTANCE hInstance,__in  HINSTANCE hPrevInstance,__in  LPSTR lpCmdLine,__in  int nCmdShow)

namespace Graphics
{
	class Window
	{
	public:
		Window(std::string& title, size_t height, size_t width, RendererTypes renderType);
		virtual ~Window();
		void Update(const Graphics::RenderFunc&& renderFunc, const UpdateFunc&& updateFunc);
		void Loop();
	private:
		void Create(std::string& title, size_t height, size_t width);

		HINSTANCE _instance;
		HWND _windowHandle;
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
		 
		std::unique_ptr<IRenderer> _renderer;
		UpdateFunc _updateFunc;
	};
}