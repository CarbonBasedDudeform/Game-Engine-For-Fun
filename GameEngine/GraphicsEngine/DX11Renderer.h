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
	class DX11Renderer final : public IRenderer 
	{
	public:
		DX11Renderer() noexcept = default;

		virtual bool CreateContext(size_t height, size_t width, HWND windowHandle) final;
		virtual void Render() final;

	protected:
		virtual void PreFrameRenderBehaviour() final;
		virtual void PostFrameRenderBehaviour() final;
		virtual void SetModelsToRender(Models const& models) final;

	private:
		

		Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain_{};
		ID3D11Device* device_{};
		ID3D11DeviceContext* context_{};
		ID3D11VertexShader* vertex_shader_{};
		ID3D11Buffer* vertices_buffer_{};

		ID3D11Buffer* index_buffer_{};
		UINT index_count_{};

		ID3D11PixelShader* pixel_shader_{};

		ID3D11DepthStencilView* depth_stencil_view_{};
		ID3D11Texture2D* depth_stencil_buffer_{};

		ID3D11RenderTargetView* render_target_{};
		ID3D11Texture2D* back_buffer_{};

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indicies;
		ID3D11InputLayout* input_layout;

		ID3DBlob* vertex_shader_blob;
		ID3DBlob* pixel_shader_blob;

	};
}