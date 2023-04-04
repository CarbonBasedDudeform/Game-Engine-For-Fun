#pragma once
#include "IUserInterfaceManager.h"

namespace Graphics::UI {

	class IMGUIManager final : public IUserInterfaceManager {
		virtual void Initialise(size_t width, size_t height, const HWND& window_handle) override final;
		virtual void Render() override final;
		virtual bool ProcessInput() override final;
	};
}
