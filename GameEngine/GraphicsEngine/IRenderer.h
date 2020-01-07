#pragma once
#include <Windows.h>
#include <functional>
#include "Model.h"

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
		virtual ~IRenderer() = default;
		virtual bool CreateContext(size_t height, size_t width, HWND windowHandle) = 0;
		virtual void Render() = 0;

		void SetModelsToRender(Models const& models) {
			sceneModels_ = models;
		}

	protected:
		virtual void PreFrameRenderBehaviour() = 0;
		virtual void PostFrameRenderBehaviour() = 0;
		Models sceneModels_{};
	};
}