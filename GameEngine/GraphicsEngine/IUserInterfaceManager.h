#pragma once
#include "IRenderer.h"

#pragma warning(push, 0) //disable warnings for external headers
#include <Windows.h>
#pragma warning(pop)

namespace Graphics::UI
{
	class IUserInterfaceManager
	{
	public:
		virtual void Initialise(size_t width, size_t height, const HWND& window_handle) = 0;
		virtual void Render() = 0;
		virtual bool ProcessInput() = 0;
	};
}