
#include "Window.h"
#include "DX11Renderer.h"

#include "hidusage.h"

namespace PAL
{
	class InputManager
	{
	public:
		InputManager() noexcept
		{
			init();
		};

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
		void init() noexcept;

		std::unordered_map<Pressable, Action> action_map_{};
		std::unordered_map<Button, Axis> axis_map_{};
	};
}


void PAL::InputManager::init() noexcept
{
	RAWINPUTDEVICE rids[2];

	rids[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rids[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rids[0].dwFlags = RIDEV_NOLEGACY;
	rids[0].hwndTarget = nullptr;

	rids[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rids[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	rids[1].dwFlags = RIDEV_NOLEGACY;
	rids[1].hwndTarget = nullptr;

	RegisterRawInputDevices(rids, 2, sizeof(rids[0]));
}

void PAL::InputManager::process(const MSG& msg)
{
	if (msg.message == WM_INPUT)
	{
		RAWINPUT raw{};
		UINT size = sizeof(raw);

		GetRawInputData(reinterpret_cast<HRAWINPUT>(msg.lParam), RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER));

		if (raw.header.dwType == RIM_TYPEKEYBOARD && action_map_.contains(raw.data.keyboard.VKey))
		{
			action_map_[raw.data.keyboard.VKey]();
		}
		
		if (raw.header.dwType == RIM_TYPEMOUSE)
		{
			axis_map_[Button::VerticalLook](static_cast<float>(raw.data.mouse.lLastY));
			axis_map_[Button::HorizontalLook](static_cast<float>(raw.data.mouse.lLastX));
		}
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

		ShowWindow(window_handle_, SW_SHOW);
		UpdateWindow(window_handle_);
		renderer_->CreateContext(height, width, window_handle_);

		ShowCursor(false);
	}

	LRESULT Window::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (message == WM_DESTROY)
		{ 
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	Window::Window(std::string& title, size_t height, size_t width, RendererTypes renderType)
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

			renderer_->Render();
		}
	}

	void Window::SetScene(Models const& current_scene_models, UpdateFunc const&& updateFunc)
	{
		update_func_ = updateFunc;
		renderer_->SetModelsToRender(current_scene_models);
	}
}