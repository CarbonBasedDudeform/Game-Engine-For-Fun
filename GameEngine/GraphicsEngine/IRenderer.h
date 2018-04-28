#pragma once
#include <Windows.h>

namespace Graphics
{
	enum RendererTypes
	{
		DX11
	};

	class IRenderer
	{
	public:
		virtual bool CreateContext(size_t height, size_t width, HWND windowHandle) = 0;
		virtual void PreFrameRenderBehaviour() = 0;
		virtual void PostFrameRenderBehaviour() = 0;
	};
}