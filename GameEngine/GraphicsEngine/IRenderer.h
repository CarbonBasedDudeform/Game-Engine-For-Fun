#pragma once
#include <Windows.h>
#include <functional>

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

		void SetRenderFunc(const RenderFunc&& sceneRenderFunc) {
			_sceneRenderFunc = sceneRenderFunc;
		}

	protected:
		virtual void PreFrameRenderBehaviour() = 0;
		virtual void PostFrameRenderBehaviour() = 0;
		RenderFunc _sceneRenderFunc;
	};
}