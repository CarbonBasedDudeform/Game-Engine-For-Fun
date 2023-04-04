#pragma once
#include "Model.h"
#include "ICamera.h"

#pragma warning(push, 0) //disable warnings for external headers
#include <Windows.h>
#include <functional>
#pragma warning(pop)

namespace Graphics
{
	enum RendererTypes
	{
		DX11
	};

	using RenderFunc = std::function<void()>;
	using UpdateFunc = std::function<void()>;

	class IRenderer
	{
	public:
		virtual bool CreateContext(size_t height, size_t width, HWND windowHandle) = 0;
		virtual void Render() = 0;
		virtual void MoveCamera(const ICamera& pos) = 0;

		virtual void SetModelsToRender(Models const& models) {
			scene_models_ = models;
		}

	protected:
		virtual void PreFrameRenderBehaviour() = 0;
		virtual void PostFrameRenderBehaviour() = 0;
	
	protected:
		Models scene_models_{};
	};
}