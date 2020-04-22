#include "DX11Renderer.h"

#pragma warning(push, 0) //disable warnings for external headers
#include <filesystem>   
#include <iostream> //todo: switch to some non-basic bitch logger
#include <DirectXMath.h>
#pragma warning(pop) //enable warnings again


namespace Graphics
{
	ID3DBlob * load_shader_blob(std::filesystem::path const& path_to_cso)
	{
		ID3DBlob* shader_blob{ };
		auto const the_path = std::filesystem::current_path() / path_to_cso;
		D3DReadFileToBlob(the_path.c_str(), &shader_blob);
		if (!shader_blob)
		{
			auto err_msg = std::wstring{ L"Failed to load shader: " };
			err_msg += path_to_cso.wstring();
			err_msg += L"\n";
			OutputDebugStringW(err_msg.c_str());
		}
		else
		{
			auto err_msg = std::wstring{ L"Successfully loaded shader: " };
			err_msg += path_to_cso.wstring();
			err_msg += L"\n";
			OutputDebugStringW(err_msg.c_str());
		}

		return shader_blob;
	}

	bool DX11Renderer::CreateContext(size_t height, size_t width, HWND windowHandle)
	{
		D3D_FEATURE_LEVEL  const featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};
		
		world = DirectX::XMMatrixIdentity();
		projection = DirectX::XMMatrixPerspectiveFovLH(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		//projection = DirectX::XMMatrixOrthographicLH(width, height, 0.1f, 1000.0f);

		DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 5.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		view = DirectX::XMMatrixLookAtLH(eye, lookAt, up);

		


		D3D_FEATURE_LEVEL selectedFeatureLevel{};

		UINT const createDeviceFlags = 0;
		HRESULT hr;
		if (FAILED((hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &device_, &selectedFeatureLevel, &context_))))
		{
			return false;
		}

		//UINT qualityLevels[1];
		//device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, qualityLevels);
		

		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
		swap_chain_desc.SampleDesc.Count = 1;
		//swapChainDesc.SampleDesc.Quality = 0;// qualityLevels[0];
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_desc.BufferCount = 1;

		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.Width = width;
		swap_chain_desc.BufferDesc.Height = height;
		swap_chain_desc.OutputWindow = windowHandle;
		swap_chain_desc.Windowed = true;


		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
		ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
		fullScreenDesc.RefreshRate.Numerator = 60;
		fullScreenDesc.RefreshRate.Denominator = 1;
		fullScreenDesc.Windowed = false;

		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain_, &device_, NULL, &context_);
		
		swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer_));
		
		device_->CreateRenderTargetView(back_buffer_, nullptr, &render_target_);
		back_buffer_->Release();
		
		D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
		ZeroMemory(&depthStencilTextureDesc, sizeof(depthStencilTextureDesc));
		depthStencilTextureDesc.Width = width;
		depthStencilTextureDesc.Height = height;
		depthStencilTextureDesc.MipLevels = 1;
		depthStencilTextureDesc.ArraySize = 1;
		depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilTextureDesc.SampleDesc.Count = 1;
		depthStencilTextureDesc.SampleDesc.Quality = 0;
		depthStencilTextureDesc.CPUAccessFlags = 0;
		depthStencilTextureDesc.MiscFlags = 0;
		
		device_->CreateTexture2D(&depthStencilTextureDesc, nullptr,&depth_stencil_buffer_);

		
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
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

		device_->CreateDepthStencilState(&depthStencilDesc, &depthStencilState_);
		context_->OMSetDepthStencilState(depthStencilState_, 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		device_->CreateDepthStencilView(depth_stencil_buffer_, &depthStencilViewDesc, &depth_stencil_view_);
		
		
		context_->OMSetRenderTargets(1, &render_target_, depth_stencil_view_);

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		context_->RSSetViewports(1, &viewport);

		//set up shaders
		vertex_shader_blob = load_shader_blob(L"TexturedVertexShaderDx11.cso");
		pixel_shader_blob = load_shader_blob(L"TexturedPixelShaderDx11.cso");
		
		device_->CreateVertexShader(vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), nullptr, &vertex_shader_);
		device_->CreatePixelShader(pixel_shader_blob->GetBufferPointer(), pixel_shader_blob->GetBufferSize(), nullptr, &pixel_shader_);

		context_->VSSetShader(vertex_shader_, nullptr, 0);
		context_->PSSetShader(pixel_shader_, nullptr, 0);

		//set up input layout
		ID3D11InputLayout* input_layout;
		D3D11_INPUT_ELEMENT_DESC input_desc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		 
		device_->CreateInputLayout(input_desc, 2, vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), &input_layout); // 3 = sizeof(input_desc) / sizeof(d3d11_input_element_desc) ?
		context_->IASetInputLayout(input_layout);

		//set raster state
		//D3D11_RASTERIZER_DESC raster_desc;
		//ZeroMemory(&raster_desc, sizeof(raster_desc));
		//raster_desc.CullMode = D3D11_CULL_BACK;
		////raster_desc.DepthClipEnable = true;
		//raster_desc.FrontCounterClockwise = false;
		//raster_desc.FillMode = D3D11_FILL_SOLID;
		//
		//ID3D11RasterizerState* raster_state;
		//device_->CreateRasterizerState(&raster_desc, &raster_state);
		//context_->RSSetState(raster_state);

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

		// Create the texture sampler state.
		device_->CreateSamplerState(&samplerDesc, &m_sampleState);
		context_->PSSetSamplers(0, 1, &m_sampleState);
		
		return true;
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
		DirectX::XMVECTOR eye = camera.getEye();
		DirectX::XMVECTOR lookAt = camera.getLookAt();
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		view = DirectX::XMMatrixLookAtLH(eye, lookAt, up);
	}

	void DX11Renderer::createVertexBuffer(ID3D11Buffer** buffer, const std::vector<Vertex>& vertices)
	{
		D3D11_SUBRESOURCE_DATA vertex_buffer_sub_resource;
		vertex_buffer_sub_resource.pSysMem = vertices.data();

		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.ByteWidth = sizeof(Vertex) * vertices.size();
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device_->CreateBuffer(&buffer_desc, &vertex_buffer_sub_resource, buffer);
	}

	void DX11Renderer::createIndicesBuffer(ID3D11Buffer** buffer, const std::vector<unsigned int>& indices)
	{
		D3D11_SUBRESOURCE_DATA index_buffer_sub_resource;
		index_buffer_sub_resource.pSysMem = indices.data();

		D3D11_BUFFER_DESC index_buffer_desc;
		ZeroMemory(&index_buffer_desc, sizeof(index_buffer_desc));
		index_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		index_buffer_desc.ByteWidth = sizeof(indices[0]) * indices.size();
		index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device_->CreateBuffer(&index_buffer_desc, &index_buffer_sub_resource, buffer);
	}

	void DX11Renderer::createTexture(ID3D11Texture2D** texture, ID3D11ShaderResourceView** texture_view, const Image& const material)
	{
		if (texture_pool_.count(material.Id))
		{
			*texture = texture_pool_[material.Id].texture;
			*texture_view = texture_pool_[material.Id].view;
			return;
		}

		D3D11_TEXTURE2D_DESC texDesc;
		//if i forget this i am a horrible, horrible little gremlin.
		texDesc.Height = material.Height;
		texDesc.Width = material.Width;
		texDesc.MipLevels = 0;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		auto hr = device_->CreateTexture2D(&texDesc, NULL, texture);
		if (hr != S_OK) throw std::exception{};

		auto rowPitch = (material.Width * 4) * sizeof(unsigned char);
		context_->UpdateSubresource(*texture, 0, NULL, material.Data, rowPitch, 0);
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		device_->CreateShaderResourceView(*texture, &srvDesc, texture_view);
		context_->GenerateMips(*texture_view);

		texture_pool_[material.Id] = { *texture, *texture_view };
	}

	void DX11Renderer::createConstantBuffer(ID3D11Buffer** constant_buffer)
	{
		D3D11_BUFFER_DESC cbd{ 0 };
		cbd.ByteWidth = sizeof(ConstantBuffer);
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		device_->CreateBuffer(&cbd, 0, constant_buffer);
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
				if (!to_render.texture && texture  && texture->Data)
				{
					createTexture(&to_render.texture, &to_render.texture_view, *texture);
				}

				if (!to_render.vertices_buffer)
				{
					createVertexBuffer(&to_render.vertices_buffer, to_render.vertices);
					to_render.index_count = to_render.vertices.size();
				}
				
				//if (!to_render.index_buffer)
				//{
				//	createIndicesBuffer(&to_render.index_buffer, to_render.idxs);
				//}

				if (!to_render.constant_buffer)
				{
					createConstantBuffer(&to_render.constant_buffer);
				}

				auto const stride = UINT{ sizeof(Vertex) };
				auto const offset = UINT{ 0 };
				
				context_->IASetVertexBuffers(0, 1, &to_render.vertices_buffer, &stride, &offset);
				context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				//context_->IASetIndexBuffer(to_render.index_buffer, DXGI_FORMAT_R32_UINT, 0);

				model.constant_buffer.world = DirectX::XMMatrixTranspose(world);
				model.constant_buffer.view = DirectX::XMMatrixTranspose(view);
				model.constant_buffer.projection = DirectX::XMMatrixTranspose(projection);
				context_->UpdateSubresource(to_render.constant_buffer, 0, 0, &model.constant_buffer, 0, 0);
				
				context_->VSSetConstantBuffers(0, 1, &to_render.constant_buffer);
				
				if (to_render.texture_view) context_->PSSetShaderResources(0, 1, &to_render.texture_view);

				context_->Draw(to_render.index_count, 0);
			}
		}
		
		PostFrameRenderBehaviour();
	}
}