#include "DX11Renderer.h"

#pragma warning(push, 0) //disable warnings for external headers
#include <filesystem>   
#include <iostream> //todo: switch to some non-basic bitch logger
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
		//viewport.MinDepth = 0.0f;
		//viewport.MaxDepth = 1.0f;

		context_->RSSetViewports(1, &viewport);
		
		ID3DBlob* vertex_shader_blob = load_shader_blob(L"DefaultVertexShaderDx11.cso");
		ID3DBlob* pixel_shader_blob = load_shader_blob(L"DefaultPixelShaderDx11.cso");

		
		device_->CreateVertexShader(vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), nullptr, &vertex_shader_);
		device_->CreatePixelShader(pixel_shader_blob->GetBufferPointer(), pixel_shader_blob->GetBufferSize(), nullptr, &pixel_shader_);

		context_->VSSetShader(vertex_shader_, nullptr, 0);
		context_->PSSetShader(pixel_shader_, nullptr, 0);

		//set up input layout

		ID3D11InputLayout* input_layout;
		D3D11_INPUT_ELEMENT_DESC input_desc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		device_->CreateInputLayout(input_desc, 2, vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), &input_layout);
		context_->IASetInputLayout(input_layout);


		//set up vertex buffer
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.ByteWidth = sizeof(Vertex) * 3;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device_->CreateBuffer(&buffer_desc, nullptr, &vertices_buffer_);
		
		D3D11_MAPPED_SUBRESOURCE mapped_subresource;
		context_->Map(vertices_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
		memcpy(mapped_subresource.pData, OurVertices, sizeof(OurVertices));
		context_->Unmap(vertices_buffer_, 0);

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

	void DX11Renderer::Render()
	{
		PreFrameRenderBehaviour();
		auto const stride = UINT{ sizeof(Vertex) };
		auto const offset = UINT{ 0 };
		context_->IASetVertexBuffers(0, 1, &vertices_buffer_, &stride, &offset);
		context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context_->Draw(3, 0);

		//for (auto& current_model : scene_models_)
		{
			//render the model...

		}
		PostFrameRenderBehaviour();
	}
}