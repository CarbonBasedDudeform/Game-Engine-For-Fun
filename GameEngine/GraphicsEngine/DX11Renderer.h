#pragma once
#include "IRenderer.h"
#include <dxgi1_2.h>
#include <D3D11.h>

namespace Graphics
{
	class DX11Renderer : public IRenderer
	{
	public:
		DX11Renderer();
		~DX11Renderer();
		virtual bool CreateContext(size_t height, size_t width, HWND windowHandle);
		virtual void PreFrameRenderBehaviour();
		virtual void PostFrameRenderBehaviour();
	private:
		IDXGISwapChain1 * _swapChain;
		ID3D11RenderTargetView * _renderTarget;
		ID3D11Texture2D * _depthStencilBuffer;
		ID3D11DepthStencilView * _depthStencilView;
	};
}