#include "IMGUIManager.h"

#include "ImGui/imgui.h"
#include "ImGui/Backends/imgui_impl_win32.h"
#include "ImGui/Backends/imgui_impl_dx11.h"

#include "hidusage.h"
#include "PointLightUI.h"
#include "Outliner.h"

namespace Graphics::UI {
	void IMGUIManager::Initialise(size_t width, size_t height, const HWND& window_handle) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.DisplaySize.x = width;
		io.DisplaySize.y = height;
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(window_handle);
	}

	void IMGUIManager::Render() {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		::UI::Light::Point::render();
		::UI::World::Outliner::render();

		ImGui::Render();
	}

	bool IMGUIManager::ProcessInput() {
		ImGuiIO& io = ImGui::GetIO();
		return io.WantCaptureMouse || io.WantCaptureKeyboard;
	}
}