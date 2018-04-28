#include "Window.h"
#pragma comment(lib, "d3d11.lib")

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
		_windowHandle = CreateWindow(title.c_str(), title.c_str(), WS_BORDER, 0, 0, width, height, nullptr, nullptr, _instance, nullptr);

		ShowWindow(_windowHandle, SW_SHOW);
		UpdateWindow(_windowHandle);

		D3D_FEATURE_LEVEL featureLevels[] = {			
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		ID3D11Device* direct3DDevice;
		D3D_FEATURE_LEVEL selectedFeatureLevel;
		ID3D11DeviceContext* direct3DDeviceContext;
		UINT createDeviceFlags = 0;
		HRESULT hr;
		if (FAILED((hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &direct3DDevice, &selectedFeatureLevel, &direct3DDeviceContext))))
		{
			return;
		}
		
		UINT qualityLevels[1];
		direct3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, qualityLevels);


		IDXGIDevice * dxgiDevice;
		direct3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		IDXGIAdapter * dxgiAdapter;
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
		IDXGIFactory2 * dxgiFactory;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.Width = width;
		swapChainDesc.Height = height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;// qualityLevels[0];
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.BufferCount = 1;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
		ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
		fullScreenDesc.RefreshRate.Numerator = 60;
		fullScreenDesc.RefreshRate.Denominator = 1;
		fullScreenDesc.Windowed = true;

		IDXGISwapChain1 * swapChain;
		auto blep = dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, _windowHandle, &swapChainDesc, &fullScreenDesc, nullptr, &swapChain);

		ID3D11Texture2D* backBuffer;
		auto res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

		direct3DDevice->CreateRenderTargetView(backBuffer, nullptr, &_renderTarget);

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		direct3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
		direct3DDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		direct3DDeviceContext->RSSetViewports(1, &viewport);
		swapChain->Present(0, 0);
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
	void Window::Update(const std::function<void()> gameLoop)
	{
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
				gameLoop();
			}
		}
	}
}