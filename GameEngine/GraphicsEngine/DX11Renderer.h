#pragma once
#include "IRenderer.h"

#pragma warning(push, 0) //disable warnings for external headers
#include <dxgi1_2.h>
#include <D3D11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <wrl/client.h>
#include <map>
#pragma warning(pop) //enable warnings again

struct Renderable {
	ID3D11Buffer* vertices_buffer;
	ID3D11ShaderResourceView* texture_view;
	ID3D11Texture2D* texture;
	ID3D11Buffer* constant_buffer;
	UINT index_count;
	std::vector<Graphics::Vertex> vertices;
};

using Renderables = std::map<int, Renderable>; //<id, target>

namespace Graphics
{
	class DX11Renderer final : public IRenderer 
	{
	public:
		DX11Renderer() = default;

		virtual bool CreateContext(size_t height, size_t width, HWND windowHandle) final;
		virtual void Render() final;

	protected:
		virtual void PreFrameRenderBehaviour() final;
		virtual void PostFrameRenderBehaviour() final;
		virtual void SetModelsToRender(Models const& models) final;
		void MoveCamera(const ICamera& camera) final;

	private:
		void createVertexBuffer(ID3D11Buffer** buffer, const std::vector<Vertex>& vertices);
		void createIndicesBuffer(ID3D11Buffer** buffer, const std::vector<unsigned int>& indices);
		void createTexture(ID3D11Texture2D** texture, ID3D11ShaderResourceView** texture_view, const Image& material);
		void createConstantBuffer(ID3D11Buffer** constant_buffer);
		
	private:
		struct TextureStore
		{
			ID3D11Texture2D* texture;
			ID3D11ShaderResourceView* view;
		};

		Renderables renderables_;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain_{};

		ID3D11Device* device_{};
		ID3D11DeviceContext* context_{};
		ID3D11VertexShader* vertex_shader_{};
		
		ID3D11PixelShader* pixel_shader_{};

		ID3D11DepthStencilView* depth_stencil_view_{};
		ID3D11Texture2D* depth_stencil_buffer_{};

		ID3D11RenderTargetView* render_target_{};
		ID3D11Texture2D* back_buffer_{};

		DirectX::XMMATRIX world{};
		DirectX::XMMATRIX view{};
		DirectX::XMMATRIX projection{};

		std::map<int, TextureStore> texture_pool_{};
	};
}