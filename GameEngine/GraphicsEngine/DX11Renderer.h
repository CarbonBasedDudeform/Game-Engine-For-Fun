#pragma once
#include "IRenderer.h"
#include <dxgi1_2.h>
#include <D3D11.h>
#include <memory>
#include <wrl/client.h>

namespace Graphics
{
	class DX11Renderer : public IRenderer
	{
	public:
		DX11Renderer();
		~DX11Renderer();
		virtual bool CreateContext(size_t height, size_t width, HWND windowHandle);
		virtual void Render();

	private:
		virtual void PreFrameRenderBehaviour();
		virtual void PostFrameRenderBehaviour();
		Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
		ID3D11Device* device_{};
		ID3D11DeviceContext* context_{};
	};
}