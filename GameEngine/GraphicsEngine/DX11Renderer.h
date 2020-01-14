#pragma once
#include "IRenderer.h"

#pragma warning(push, 0) //disable warnings for external headers
#include <dxgi1_2.h>
#include <D3D11.h>
#include <d3dcompiler.h>
#include <memory>
#include <wrl/client.h>
#pragma warning(pop) //enable warnings again

namespace Graphics
{
	class DX11Renderer : public IRenderer
	{
	public:
		DX11Renderer();
		~DX11Renderer();
		virtual bool CreateContext(size_t height, size_t width, HWND windowHandle);
		virtual void Render();

	protected:
		virtual void PreFrameRenderBehaviour();
		virtual void PostFrameRenderBehaviour();

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain1> swap_chain_;
		ID3D11Device* device_{};
		ID3D11DeviceContext* context_{};
	};
}