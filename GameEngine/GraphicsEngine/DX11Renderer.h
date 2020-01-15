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
	//struct Color //todo: make do good.
	//{
	//	float r, g, b, a;
	//};

	using Color = float[4];
	struct Vertex //todo: switch to directXMath and get dem SIMD benz
	{
		float x, y, z;
		Color color;
	};

	static Vertex OurVertices[] =
	{
		{0.0f, 0.5f, 0.5f, {1.0f, 0.0f, 0.0f, 1.0f}},
		{0.45f, -0.5, 0.5f, {0.0f, 1.0f, 0.0f, 1.0f}},
		{-0.45f, -0.5f, 0.5f, {0.0f, 0.0f, 1.0f, 1.0f}}
	};

	class DX11Renderer final : public IRenderer 
	{
	public:
		DX11Renderer() noexcept = default;

		virtual bool CreateContext(size_t height, size_t width, HWND windowHandle) final;
		virtual void Render() final;

	protected:
		virtual void PreFrameRenderBehaviour() final;
		virtual void PostFrameRenderBehaviour() final;

	private:
		

		Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain_{};
		ID3D11Device* device_{};
		ID3D11DeviceContext* context_{};
		ID3D11VertexShader* vertex_shader_{};
		ID3D11Buffer* vertices_buffer_{};

		ID3D11PixelShader* pixel_shader_{};

		ID3D11DepthStencilView* depth_stencil_view_{};
		ID3D11Texture2D* depth_stencil_buffer_{};

		ID3D11RenderTargetView* render_target_{};
		ID3D11Texture2D* back_buffer_{};
	};
}