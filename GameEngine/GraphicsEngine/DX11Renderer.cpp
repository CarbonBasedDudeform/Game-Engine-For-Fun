#include "DX11Renderer.h"
#include "PointLightUI.h"

#pragma warning(push, 0) //disable warnings for external headers
#include <filesystem>   
#include <iostream> //todo: switch to some non-basic bitch logger
#include <DirectXMath.h>

#include "ImGui/Backends/imgui_impl_dx11.h"

#pragma warning(pop) //enable warnings again

namespace Graphics
{
	namespace Structures
	{
		struct StandardTextureDesc
		{
			StandardTextureDesc(const int Height, const int Width)
			{
				texDesc.Height = static_cast<UINT>(Height);
				texDesc.Width = static_cast<UINT>(Width);
				texDesc.MipLevels = 0;
				texDesc.ArraySize = 1;
				texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				texDesc.SampleDesc.Count = 1;
				texDesc.SampleDesc.Quality = 0;
				texDesc.Usage = D3D11_USAGE_DEFAULT;
				texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
				texDesc.CPUAccessFlags = 0;
				texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
			};

			D3D11_TEXTURE2D_DESC texDesc;
		};

		struct StandardShaderResource
		{
			StandardShaderResource()
			{
				srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = 1;
			};

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		};
		
		D3D_FEATURE_LEVEL static const FeatureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		struct StandardSwapChain
		{
			StandardSwapChain(size_t Height, size_t Width, HWND WindowHandle)
			{

				ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
				swap_chain_desc.SampleDesc.Count = 1;
				//swapChainDesc.SampleDesc.Quality = 0;// qualityLevels[0];
				swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
				swap_chain_desc.BufferCount = 1;

				swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				swap_chain_desc.BufferDesc.Width = static_cast<UINT>(Width);
				swap_chain_desc.BufferDesc.Height = static_cast<UINT>(Height);
				swap_chain_desc.OutputWindow = WindowHandle;
				swap_chain_desc.Windowed = true;
			};

			DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		};

		struct StandardFullscreenDesc
		{
			StandardFullscreenDesc()
			{
				ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
				fullScreenDesc.RefreshRate.Numerator = 60;
				fullScreenDesc.RefreshRate.Denominator = 1;
				fullScreenDesc.Windowed = false;
			};

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
		};

		struct StandardDepthStencilTexture
		{
			StandardDepthStencilTexture(size_t Height, size_t Width)
			{
				ZeroMemory(&depthStencilTextureDesc, sizeof(depthStencilTextureDesc));
				depthStencilTextureDesc.Width = static_cast<UINT>(Width);
				depthStencilTextureDesc.Height = static_cast<UINT>(Height);
				depthStencilTextureDesc.MipLevels = 1;
				depthStencilTextureDesc.ArraySize = 1;
				depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
				depthStencilTextureDesc.SampleDesc.Count = 1;
				depthStencilTextureDesc.SampleDesc.Quality = 0;
				depthStencilTextureDesc.CPUAccessFlags = 0;
				depthStencilTextureDesc.MiscFlags = 0;
			};

			D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
		};

		struct StandardDepthStencil
		{
			StandardDepthStencil()
			{
				ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

				depthStencilDesc.DepthEnable = true;
				depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

				depthStencilDesc.StencilEnable = true;
				depthStencilDesc.StencilReadMask = 0xFF;
				depthStencilDesc.StencilWriteMask = 0xFF;

				depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
				depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
				depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			};

			D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		};

		struct StandardDepthStencilView
		{
			StandardDepthStencilView()
			{
				ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

				// Set up the depth stencil view description.
				depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilViewDesc.Texture2D.MipSlice = 0;
			};

			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		};
	}
	namespace General
	{
		void CreateVertexBuffer(ID3D11Device* device, ID3D11Buffer** buffer, const std::vector<Vertex>& vertices)
		{
			D3D11_SUBRESOURCE_DATA vertex_buffer_sub_resource;
			vertex_buffer_sub_resource.pSysMem = vertices.data();

			D3D11_BUFFER_DESC buffer_desc;
			ZeroMemory(&buffer_desc, sizeof(buffer_desc));
			buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
			buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			device->CreateBuffer(&buffer_desc, &vertex_buffer_sub_resource, buffer);
		}


		void CreateIndicesBuffer(ID3D11Device* device, ID3D11Buffer** buffer, const std::vector<unsigned int>& indices)
		{
			D3D11_SUBRESOURCE_DATA index_buffer_sub_resource;
			index_buffer_sub_resource.pSysMem = indices.data();

			D3D11_BUFFER_DESC index_buffer_desc;
			ZeroMemory(&index_buffer_desc, sizeof(index_buffer_desc));
			index_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
			index_buffer_desc.ByteWidth = static_cast<UINT>(sizeof(indices[0]) * indices.size());
			index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			index_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			device->CreateBuffer(&index_buffer_desc, &index_buffer_sub_resource, buffer);
		}

		void CreateConstantBuffer(ID3D11Device* device, ID3D11Buffer** constant_buffer)
		{
			D3D11_BUFFER_DESC cbd{ 0 };
			cbd.ByteWidth = sizeof(PointLightConstantBuffer);
			cbd.Usage = D3D11_USAGE_DEFAULT;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			
			device->CreateBuffer(&cbd, 0, constant_buffer);
			// Define constant buffer data
			//PointLightConstantBuffer constantBufferData = {};
			//constantBufferData.lightPosition = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			//constantBufferData.lightColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			//constantBufferData.lightIntensity = 1.0f;

			// Create constant buffer
			//D3D11_BUFFER_DESC constantBufferDesc = {};
			//constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			//constantBufferDesc.ByteWidth = sizeof(PointLightConstantBuffer);
			//constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			//constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			//constantBufferDesc.MiscFlags = 0;
			//constantBufferDesc.StructureByteStride = 0;
			//
			////D3D11_SUBRESOURCE_DATA constantBufferDataDesc = {};
			////constantBufferDataDesc.pSysMem = &constantBufferData;
			////constantBufferDataDesc.SysMemPitch = 0;
			////constantBufferDataDesc.SysMemSlicePitch = 0;
			//
			//device->CreateBuffer(&constantBufferDesc, 0, constant_buffer);

			//HRESULT hr = device->CreateBuffer(&constantBufferDesc, &constantBufferDataDesc, constant_buffer);
			//if (FAILED(hr))
			//{
			//	// Handle error
			//}
		}

		void CreateTexture(std::map<int, TextureStore>& texture_pool, ID3D11Device* device, ID3D11DeviceContext* context, ID3D11Texture2D** texture, ID3D11ShaderResourceView** texture_view, const Image& material)
		{
			const bool texture_already_created = texture_pool.count(material.Id);
			if (texture_already_created)
			{
				*texture = texture_pool[material.Id].texture;
				*texture_view = texture_pool[material.Id].view;
				return;
			}

			const auto texDesc = Structures::StandardTextureDesc{ material.Height, material.Width };
			if (device->CreateTexture2D(&texDesc.texDesc, nullptr, texture) != S_OK)
			{
				throw std::exception{ "Failed to create 2d texture" };
			}

			const auto rowPitch = (material.Width * 4) * sizeof(unsigned char);
			context->UpdateSubresource(*texture, 0, NULL, material.Data, rowPitch, 0);

			const auto srvDesc = Structures::StandardShaderResource{};
			device->CreateShaderResourceView(*texture, &srvDesc.srvDesc, texture_view);
			context->GenerateMips(*texture_view);

			texture_pool[material.Id] = { *texture, *texture_view };
		}

		ID3DBlob* LoadShaderBlob(std::filesystem::path const& path_to_cso)
		{
			ID3DBlob* shader_blob{ };
			auto const the_path = std::filesystem::current_path() / path_to_cso;
			auto hr = D3DReadFileToBlob(the_path.c_str(), &shader_blob);
			if (FAILED(hr)) {
				LPVOID lpMsgBuf;
				DWORD dw = FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR)&lpMsgBuf,
					0, NULL);

				if (dw > 0) {
					OutputDebugString((LPTSTR)lpMsgBuf);
					LocalFree(lpMsgBuf);
				}
			}

			const auto success_msg = std::format(L"{} shader: {}", !shader_blob ? L"Succesfulled loaded" : L"Failed to load", path_to_cso.wstring());
			OutputDebugStringW(success_msg.c_str());

			return shader_blob;
		}

		DirectX::XMMATRIX MakeViewMatrix()
		{
			const auto eye = DirectX::XMVectorSet(0.0f, 5.0f, 0.0f, 0.0f);
			const auto lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			const auto up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			return DirectX::XMMatrixLookAtLH(eye, lookAt, up);
		}
	}

	bool DX11Renderer::CreateContext(size_t height, size_t width, HWND windowHandle)
	{		
		InitialiseWorldViewProjection(height, width);
		CreateDevice();
		CreateSwapChain(height, width, windowHandle);
		CreateBackBuffer();
		CreateDepthStencil(height, width);
		CreateViewport(width, height);
		CreateVertexShader();
		CreatePixelShader();
		CreateSamplerState();

		ImGui_ImplDX11_Init(device_, context_);
		return true;
	}

	void DX11Renderer::CreateSamplerState()
	{
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		ID3D11SamplerState* m_sampleState;
		// Create the texture sampler state.
		device_->CreateSamplerState(&samplerDesc, &m_sampleState);
		context_->PSSetSamplers(0, 1, &m_sampleState);

	}

	void DX11Renderer::CreatePixelShader()
	{
		ID3DBlob* pixel_shader_blob = General::LoadShaderBlob(L"PointLightPixelShader.cso");
		device_->CreatePixelShader(pixel_shader_blob->GetBufferPointer(), pixel_shader_blob->GetBufferSize(), nullptr, &pixel_shader_);
		context_->PSSetShader(pixel_shader_, nullptr, 0);
	}

	void DX11Renderer::CreateVertexShader()
	{
		ID3DBlob* vertex_shader_blob = General::LoadShaderBlob(L"PointLightVertexShader.cso");
		device_->CreateVertexShader(vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), nullptr, &vertex_shader_);
		context_->VSSetShader(vertex_shader_, nullptr, 0);

		//set up input layout
		ID3D11InputLayout* input_layout;
		const D3D11_INPUT_ELEMENT_DESC input_desc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		device_->CreateInputLayout(input_desc, 3, vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), &input_layout); // 3 = sizeof(input_desc) / sizeof(d3d11_input_element_desc) ?
		context_->IASetInputLayout(input_layout);
	}

	void DX11Renderer::CreateViewport(const size_t& width, const size_t& height)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		context_->RSSetViewports(1, &viewport);
	}

	void DX11Renderer::CreateDepthStencil(const size_t& Height, const size_t& Width)
	{
		const auto depthStencilTextureDesc = Structures::StandardDepthStencilTexture{ Height, Width };
		device_->CreateTexture2D(&depthStencilTextureDesc.depthStencilTextureDesc, nullptr, &depth_stencil_buffer_);

		const auto depthStencilDesc = Structures::StandardDepthStencil{};
		ID3D11DepthStencilState* depthStencilState_;
		device_->CreateDepthStencilState(&depthStencilDesc.depthStencilDesc, &depthStencilState_);
		context_->OMSetDepthStencilState(depthStencilState_, 1);

		const auto depthStencilViewDesc = Structures::StandardDepthStencilView{};
		device_->CreateDepthStencilView(depth_stencil_buffer_, &depthStencilViewDesc.depthStencilViewDesc, &depth_stencil_view_);

		context_->OMSetRenderTargets(1, &render_target_, depth_stencil_view_);
	}

	void DX11Renderer::CreateBackBuffer()
	{
		swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer_));
		device_->CreateRenderTargetView(back_buffer_, nullptr, &render_target_);
		back_buffer_->Release();
	}

	void DX11Renderer::PreFrameRenderBehaviour()
	{
		static float const color[]{ 0.0f, 0.2f, 0.4f, 1.0f };
		context_->ClearRenderTargetView(render_target_, color);
		context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11Renderer::PostFrameRenderBehaviour()
	{
		swap_chain_->Present(0, 0);
	}

	void DX11Renderer::SetModelsToRender(Models const& models)
	{
		IRenderer::SetModelsToRender(models);
	}

	void DX11Renderer::MoveCamera(const ICamera& camera)
	{
		eye_ = camera.getEye();
		const auto lookAt = camera.getLookAt();
		const auto up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		view = DirectX::XMMatrixLookAtLH(eye_, lookAt, up);
	}

	void DX11Renderer::InitialiseWorldViewProjection(size_t Height, size_t Width)
	{
		world = DirectX::XMMatrixIdentity();
		projection = DirectX::XMMatrixPerspectiveFovLH(45.0f, (float)Width / (float)Height, 0.1f, 1000.0f);
		view = General::MakeViewMatrix();
	}

	void DX11Renderer::CreateDevice()
	{
		D3D_FEATURE_LEVEL selectedFeatureLevel{};
		UINT constexpr createDeviceFlags = 0;
		D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, Structures::FeatureLevels, ARRAYSIZE(Structures::FeatureLevels), D3D11_SDK_VERSION, &device_, &selectedFeatureLevel, &context_);
	}

	void DX11Renderer::CreateSwapChain(size_t Height, size_t Width, HWND WindowHandle)
	{
		const auto swap_chain_desc = Structures::StandardSwapChain{ Height, Width, WindowHandle };
		const auto fullscreenDesc = Structures::StandardFullscreenDesc();
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, nullptr, NULL, D3D11_SDK_VERSION, &swap_chain_desc.swap_chain_desc, &swap_chain_, &device_, NULL, &context_);
	}

	void DX11Renderer::Render()
	{
		PreFrameRenderBehaviour();

		//for each mode in scene
			//grab materials
			//for each material
				//set up resource if need be
				//grab verts for material from model
				//set up verts and idx if needs be
				//render verts with tex...simples right?

		for(auto& model : scene_models_)
		{
			auto const materials = model.getMaterialNames();

			for (std::string material_name  : materials)
			{
				const auto& texture = model.getImage(material_name);
				auto& to_render = renderables_[texture->Id];

				if (to_render.vertices.empty())
				{
					to_render.vertices = model.texture_verts_bucket[material_name];						
				}

				if (!to_render.texture && texture && texture->Data)
				{
					General::CreateTexture(texture_pool_, device_, context_, &to_render.texture, &to_render.texture_view, *texture);
				}

				if (!to_render.vertices_buffer)
				{
					General::CreateVertexBuffer(device_, &to_render.vertices_buffer, to_render.vertices);
					to_render.index_count = to_render.vertices.size();
				}
				
				if (!to_render.constant_buffer)
				{
					General::CreateConstantBuffer(device_, &to_render.constant_buffer);
				}

				auto const stride = UINT{ sizeof(Vertex) };
				auto const offset = UINT{ 0 };
				
				context_->IASetVertexBuffers(0, 1, &to_render.vertices_buffer, &stride, &offset);
				context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				
				
				model.constant_buffer.world = DirectX::XMMatrixTranspose(world);
				model.constant_buffer.view = DirectX::XMMatrixTranspose(view);
				model.constant_buffer.projection = DirectX::XMMatrixTranspose(projection);
				model.constant_buffer.eyePosition = eye_;

				model.constant_buffer.lightPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);//DirectX::XMFLOAT3(UI::Light::Point::light.point[0], UI::Light::Point::light.point[1], UI::Light::Point::light.point[2]); //DirectX::XMLoadFloat4(&myFloat4);//DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

				model.constant_buffer.intensity = 0.5f;

				context_->UpdateSubresource(to_render.constant_buffer, 0, 0, &model.constant_buffer, 0, 0);
				
				context_->VSSetConstantBuffers(0, 1, &to_render.constant_buffer);
				context_->PSSetConstantBuffers(0, 1, &to_render.constant_buffer);
				
				if (to_render.texture_view)
				{
					context_->PSSetShaderResources(0, 1, &to_render.texture_view);
				}

				context_->Draw(to_render.index_count, 0);
			}
		}
		

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		PostFrameRenderBehaviour();
	}
}