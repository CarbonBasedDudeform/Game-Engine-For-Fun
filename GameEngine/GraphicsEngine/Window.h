#pragma once
#include <string>
#include <Windows.h>
#include <dxgi1_2.h>
#include <D3D11.h>
#include <functional>


namespace Graphics
{
	class Window
	{
	public:
		Window(std::string& title, size_t height, size_t width);
		virtual ~Window();
		void Update(const std::function<void()> gameLoop);
	private:
		void Create(std::string& title, size_t height, size_t width);
		WNDCLASSEX _window;
		HINSTANCE _instance;
		HWND _windowHandle;
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		//move to renderer interface
		ID3D11RenderTargetView * _renderTarget;
		ID3D11Texture2D * _depthStencilBuffer;
		ID3D11DepthStencilView * _depthStencilView;
	};
}