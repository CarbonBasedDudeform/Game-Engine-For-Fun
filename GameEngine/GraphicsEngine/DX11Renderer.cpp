#include "DX11Renderer.h"

#pragma warning(push, 0) //disable warnings for external headers
#include <filesystem>   
#include <iostream> //todo: switch to some non-basic bitch logger
#pragma warning(pop) //enable warnings again

namespace Graphics
{
	ID3DBlob * load_shader_blob(std::filesystem::path const& path_to_cso)
	{
		ID3DBlob* shader_blob;
		D3DReadFileToBlob(path_to_cso.c_str(), &shader_blob);
		if (!shader_blob)
		{
			auto err_msg = std::wstring{ L"Failed to load shader: " };
			err_msg += path_to_cso.wstring();
			err_msg += L"\n";
			OutputDebugStringW(err_msg.c_str());
		}
		else
		{
			OutputDebugStringW(L"Succesfully loaded default vertex shader.\n");
		}

		return shader_blob;
	}

	DX11Renderer::DX11Renderer()
	{
	}


	DX11Renderer::~DX11Renderer()
	{
	}

	bool DX11Renderer::CreateContext(size_t height, size_t width, HWND windowHandle)
	{
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		
		D3D_FEATURE_LEVEL selectedFeatureLevel;

		UINT createDeviceFlags = 0;
		HRESULT hr;
		if (FAILED((hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &device_, &selectedFeatureLevel, &context_))))
		{
			return false;
		}

		UINT qualityLevels[1];
		device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, qualityLevels);


		IDXGIDevice * dxgiDevice;
		device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
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

		
		auto blep = dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, windowHandle, &swapChainDesc, &fullScreenDesc, nullptr, &swap_chain_);

		ID3D11Texture2D* backBuffer;
		auto res = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
		ID3D11RenderTargetView * renderTarget;
		device_->CreateRenderTargetView(backBuffer, nullptr, &renderTarget);
		
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

		ID3D11Texture2D * depthStencilBuffer;
		device_->CreateTexture2D(&depthStencilDesc, nullptr,&depthStencilBuffer);
		ID3D11DepthStencilView * depthStencilView;
		device_->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		context_->RSSetViewports(1, &viewport);
		
		std::unique_ptr<ID3DBlob> vertex_shader_blob{ load_shader_blob(L"DefaultVertexShaderDx11.cso") };
		std::unique_ptr<ID3DBlob> pixel_shader_blob{ load_shader_blob(L"DefaultPixelShaderDx11.cso") };

		return true;
	}

	void DX11Renderer::PreFrameRenderBehaviour()
	{

	}

	void DX11Renderer::PostFrameRenderBehaviour()
	{
		swap_chain_->Present(0, 0);
	}

	void DX11Renderer::Render()
	{
		PreFrameRenderBehaviour();
		for (auto& current_model : scene_models_)
		{
			//render the model...

		}
		PostFrameRenderBehaviour();
	}
}