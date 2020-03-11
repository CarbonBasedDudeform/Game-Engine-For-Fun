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
		swap_chain_desc.SampleDesc.Count = 4;
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
		fullScreenDesc.Windowed = true;

		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain_, &device_, NULL, &context_);
		
		swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer_));
		
		device_->CreateRenderTargetView(back_buffer_, nullptr, &render_target_);
		back_buffer_->Release();
		
		//D3D11_TEXTURE2D_DESC depthStencilDesc;
		//ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		//depthStencilDesc.Width = width;
		//depthStencilDesc.Height = height;
		//depthStencilDesc.MipLevels = 1;
		//depthStencilDesc.ArraySize = 1;
		//depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		//depthStencilDesc.SampleDesc.Count = 1;
		//depthStencilDesc.SampleDesc.Quality = 0;
		//
		//device_->CreateTexture2D(&depthStencilDesc, nullptr,&depth_stencil_buffer_);
		//device_->CreateDepthStencilView(depth_stencil_buffer_, nullptr, &depth_stencil_view_);
		//
		//
		//context_->OMSetRenderTargets(1, &render_target_, depth_stencil_view_);
		context_->OMSetRenderTargets(1, &render_target_, nullptr);

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
		D3D11_RASTERIZER_DESC raster_desc;
		ZeroMemory(&raster_desc, sizeof(raster_desc));
		raster_desc.CullMode = D3D11_CULL_BACK;
		//raster_desc.DepthClipEnable = true;
		raster_desc.FrontCounterClockwise = false;
		raster_desc.FillMode = D3D11_FILL_SOLID;
		
		ID3D11RasterizerState* raster_state;
		device_->CreateRasterizerState(&raster_desc, &raster_state);
		context_->RSSetState(raster_state);

		return true;
	}

	void DX11Renderer::PreFrameRenderBehaviour()
	{
		static float const color[]{ 0.0f, 0.2f, 0.4f, 1.0f };
		context_->ClearRenderTargetView(render_target_, color);
	}

	void DX11Renderer::PostFrameRenderBehaviour()
	{
		swap_chain_->Present(0, 0);
	}

	void DX11Renderer::SetModelsToRender(Models const& models)
	{
		IRenderer::SetModelsToRender(models);

		//if there is nothing, then bye bye
		if (models.empty()) return;


	}

	void DX11Renderer::Render()
	{
		PreFrameRenderBehaviour();

		for (auto& cur_model : scene_models_) 
		{
			for (auto& mesh : cur_model.getMeshes())
			{
				if (renderables_.find(mesh.id) == renderables_.end()) 
				{
					auto& new_renderable = renderables_[mesh.id];
					new_renderable.index_count = mesh.indices.size();

					//create vertex buffer

					D3D11_SUBRESOURCE_DATA vertex_buffer_sub_resource;
					vertex_buffer_sub_resource.pSysMem = mesh.vertices.data();

					D3D11_BUFFER_DESC buffer_desc;
					ZeroMemory(&buffer_desc, sizeof(buffer_desc));
					buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
					buffer_desc.ByteWidth = sizeof(Vertex) * mesh.vertices.size();
					buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

					device_->CreateBuffer(&buffer_desc, &vertex_buffer_sub_resource, &new_renderable.vertices_buffer);

					//create index buffer

					D3D11_SUBRESOURCE_DATA index_buffer_sub_resource;
					index_buffer_sub_resource.pSysMem = mesh.indices.data();

					D3D11_BUFFER_DESC index_buffer_desc;
					ZeroMemory(&index_buffer_desc, sizeof(index_buffer_desc));
					index_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
					index_buffer_desc.ByteWidth = sizeof(mesh.indices[0]) * new_renderable.index_count;
					index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
					index_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

					device_->CreateBuffer(&index_buffer_desc, &index_buffer_sub_resource, &new_renderable.index_buffer);

					

					auto& texture = mesh.texture;

					if (texture->Data) {


						D3D11_TEXTURE2D_DESC texDesc;
						//if i forget this i am a horrible, horrible little gremlin.
						texDesc.Height = texture->Height;
						texDesc.Width = texture->Width;
						texDesc.MipLevels = 0;
						texDesc.ArraySize = 1;
						texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
						texDesc.SampleDesc.Count = 1;
						texDesc.SampleDesc.Quality = 0;
						texDesc.Usage = D3D11_USAGE_DEFAULT;
						texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
						texDesc.CPUAccessFlags = 0;
						texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

						auto hr = device_->CreateTexture2D(&texDesc, NULL, &new_renderable.texture);
						if (hr != S_OK) throw std::exception{};

						auto rowPitch = (texture->Width * 4) * sizeof(unsigned char);
						context_->UpdateSubresource(new_renderable.texture, 0, NULL, texture->Data, rowPitch, 0);
						D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
						srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
						srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						srvDesc.Texture2D.MostDetailedMip = 0;
						srvDesc.Texture2D.MipLevels = 1;

						device_->CreateShaderResourceView(new_renderable.texture, &srvDesc, &new_renderable.texture_view);
						context_->GenerateMips(new_renderable.texture_view);

						{
							D3D11_BUFFER_DESC cbd{ 0 }; 
							cbd.ByteWidth = 16;
							cbd.Usage = D3D11_USAGE_DEFAULT;
							cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

							device_->CreateBuffer(&cbd, 0, &new_renderable.constant_buffer);
							context_->UpdateSubresource(new_renderable.constant_buffer, 0, 0, &cur_model.position, 0, 0);
						}
					}
				}

				auto& to_render = renderables_[mesh.id];
				
				auto const stride = UINT{ sizeof(Vertex) };
				auto const offset = UINT{ 0 };

				context_->VSSetConstantBuffers(0, 1, &to_render.constant_buffer);
				context_->IASetVertexBuffers(0, 1, &to_render.vertices_buffer, &stride, &offset);
				context_->IASetIndexBuffer(to_render.index_buffer, DXGI_FORMAT_R32_UINT, 0);
				context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				context_->PSSetShaderResources(0, 1, &to_render.texture_view);

				context_->DrawIndexed(to_render.index_count, 0, 0);
			}
		}
		
		
		PostFrameRenderBehaviour();
	}
}