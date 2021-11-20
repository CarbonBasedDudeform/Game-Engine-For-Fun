
#include "Window.h"
#include "windowsx.h"
#include "DX11Renderer.h"

#include "hidusage.h"

#include "ImGui/imgui.h"
#include "ImGui/Backends/imgui_impl_win32.h"
#include "ImGui/Backends/imgui_impl_dx11.h"

namespace PAL
{
	class InputManager
	{
	public:
		using Pressable = int;
		enum Button
		{
			VerticalLook,
			HorizontalLook
		};

		using Action = std::function<void()>;
		using Axis = std::function<void(float)>;
		
		
		void registerAction(Pressable button, const Action&& action);
		void registerAxis(Button button, const Axis&& axis);
		void process(const MSG& msg);

	private:
		int last_X_{};
		int last_Y_{};

		std::unordered_map<Pressable, Action> action_map_{};
		std::unordered_map<Button, Axis> axis_map_{};
	};
}

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void PAL::InputManager::process(const MSG& msg)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse || io.WantCaptureKeyboard) return;

	if (msg.message == WM_MOUSEMOVE)
	{
		int x = GET_X_LPARAM(msg.lParam);
		int y = GET_Y_LPARAM(msg.lParam);

		if (x != last_X_)
		{
			int delta = x - last_X_;
			axis_map_[Button::HorizontalLook](static_cast<float>(delta));
			last_X_ = x;
		}

		if (y != last_Y_)
		{
			int delta = y - last_Y_;
			axis_map_[Button::VerticalLook](static_cast<float>(delta));
			last_Y_ = y;
		}
	}
	else if (msg.message == WM_KEYDOWN)
	{
		action_map_[msg.wParam]();
	}
}

void PAL::InputManager::registerAction(Pressable button, const Action&& action)
{
	action_map_[button] = action;
}

void PAL::InputManager::registerAxis(Button button, const Axis&& axis)
{
	axis_map_[button] = axis;
}


namespace Graphics
{
	void Window::Create(std::string const& title, size_t height, size_t width)
	{
		WNDCLASSEX window;
		ZeroMemory(&window, sizeof(window));
		window.cbSize = sizeof(WNDCLASSEX);
		window.style = CS_CLASSDC;
		window.lpfnWndProc = WndProc;
		window.hInstance = instance_;
		window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		window.lpszClassName = title.c_str();

		RegisterClassEx(&window);
		window_handle_ = CreateWindow(title.c_str(), title.c_str(), WS_BORDER, 0, 0, static_cast<int>(width), static_cast<int>(height), nullptr, nullptr, instance_, nullptr);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.DisplaySize.x = width;
		io.DisplaySize.y = height;
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(window_handle_);

		ShowWindow(window_handle_, SW_SHOW);
		UpdateWindow(window_handle_);

		RECT clientRect;
		GetClientRect(window_handle_, &clientRect);

		renderer_->CreateContext(clientRect.bottom, clientRect.right, window_handle_);

		//ShowCursor(false);

	}

	LRESULT Window::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (ImGui_ImplWin32_WndProcHandler(windowHandle, message, wParam, lParam))
		{
			return TRUE;
		}

		if (message == WM_DESTROY)
		{ 
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	Window::Window(const std::string& title, size_t height, size_t width, RendererTypes renderType)
		: input_manager_(std::make_shared<PAL::InputManager>())
	{
		switch (renderType)
		{
		case RendererTypes::DX11:
			renderer_ = std::make_unique<DX11Renderer>();
		}
		Create(title, height, width);
	}
	
	void Window::Loop() {
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		input_manager_->registerAction(VK_ESCAPE, [&]() noexcept
			{
				std::quick_exit(0);
			});

		input_manager_->registerAction('S', [&]()
			{
				camera_.moveBack();
				renderer_->MoveCamera(camera_);
			});

		input_manager_->registerAction('W', [&]()
			{
				camera_.moveForward();
				renderer_->MoveCamera(camera_);
			});

		input_manager_->registerAction('A', [&]()
			{
				camera_.moveLeft();
				renderer_->MoveCamera(camera_);
			});

		input_manager_->registerAction('D', [&]()
			{
				camera_.moveRight();
				renderer_->MoveCamera(camera_);
			});

		input_manager_->registerAxis(PAL::InputManager::VerticalLook, [&](float delta)
			{
				camera_.rotatePitch(delta);
				renderer_->MoveCamera(camera_);
			});

		input_manager_->registerAxis(PAL::InputManager::HorizontalLook, [&](float delta)
			{
				camera_.rotateYaw(delta);
				renderer_->MoveCamera(camera_);
			});

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				input_manager_->process(msg);
			}

			

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			bool show = true;
			ImGui::ShowDemoWindow(&show);
			ImGui::Render();

			renderer_->Render();
		}
	}

	void Window::SetScene(Models const& current_scene_models, UpdateFunc const&& updateFunc)
	{
		update_func_ = updateFunc;
		renderer_->SetModelsToRender(current_scene_models);
	}
}