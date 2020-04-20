#include "Window.h"
#include "DX11Renderer.h"

#include <gainput/gainput.h>

namespace PAL
{
	class InputManager
	{
	public:
		enum Button
		{
			Down,
			Up,
			Left,
			Right,
			VerticalLook,
			HorizontalLook
		};

		using Action = std::function<void()>;
		using Axis = std::function<void(float)>;
		
		void init();
		void registerAction(Button button, const Action&& action);
		void registerAxis(Button button, const Axis&& axis);
		void process(const MSG& msg);
		void update();

	private:
		std::unordered_map<Button, std::vector<Action>> action_map_{};
		std::unordered_map<Button, std::vector<Axis>> axis_map_{};

		gainput::InputManager manager_{};
		gainput::InputMap map_{ manager_ };
		int supress_count_{};
	};
}


void PAL::InputManager::init()
{

	manager_.SetDisplaySize(1920, 1080);
	const gainput::DeviceId keyboardId = manager_.CreateDevice<gainput::InputDeviceKeyboard>();
	const gainput::DeviceId mouseId = manager_.CreateDevice<gainput::InputDeviceMouse>();
	const gainput::DeviceId padId = manager_.CreateDevice<gainput::InputDevicePad>();
	const gainput::DeviceId touchId = manager_.CreateDevice<gainput::InputDeviceTouch>();
	//keyboard
	map_.MapBool(Down, keyboardId, gainput::KeyS);
	map_.MapBool(Up, keyboardId, gainput::KeyW);
	map_.MapBool(Left, keyboardId, gainput::KeyA);
	map_.MapBool(Right, keyboardId, gainput::KeyD);
	map_.MapFloat(VerticalLook, mouseId, gainput::MouseAxisY);
	map_.MapFloat(HorizontalLook, mouseId, gainput::MouseAxisX);
}

void PAL::InputManager::process(const MSG& msg)
{
	manager_.Update();
	manager_.HandleMessage(msg);
}

void PAL::InputManager::registerAction(Button button, const Action&& action)
{
	action_map_[button].emplace_back(action);
}

void PAL::InputManager::registerAxis(Button button, const Axis&& axis)
{
	axis_map_[button].emplace_back(axis);
}

void PAL::InputManager::update()
{
	for (auto&& [button, actions] : action_map_)
	{
		if (map_.GetBool(button))
		{
			for (auto& action : actions)
			{
				action();
			}
		}
	}

	for (auto&& [button, axises] : axis_map_)
	{
		if (supress_count_ >= 50 && map_.GetFloatDelta(button) != 0.0)
		{
			for (auto& axis : axises)
			{
				axis(map_.GetFloatDelta(button));
			}

			supress_count_ = 0;
		}

		supress_count_++;
	}
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
		window_handle_ = CreateWindow(title.c_str(), title.c_str(), WS_BORDER, 0, 0, width, height, nullptr, nullptr, instance_, nullptr);

		ShowWindow(window_handle_, SW_SHOW);
		UpdateWindow(window_handle_);
		renderer_->CreateContext(height, width, window_handle_);

		ShowCursor(false);

	}

	LRESULT Window::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
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

	float magnitude(const Vector& vec)
	{
		return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	}

	void normalize(Vector& vec)
	{
		auto const mag = magnitude(vec);
		vec.x /= mag;
		vec.y /= mag;
		vec.z /= mag;
	}

	Vector cross(const Vector& a, const Vector& b)
	{
		return {
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z ,
				a.x * b.y - a.y * b.x
		};
	}

	

	void Window::recenter_cursor()
	{
		POINT mouse_pos{};
		GetCursorPos(&mouse_pos);
		RECT window_rect;
		GetWindowRect(window_handle_, &window_rect);
		SetCursorPos((window_rect.left + window_rect.right) / 2, (window_rect.bottom + window_rect.top) / 2);
	}

	void Window::Loop() {
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		//set up input manager
		input_manager_->init();

		input_manager_->registerAction(PAL::InputManager::Down, [&]()
			{
				camera_.moveBack();
				renderer_->MoveCamera(camera_);
			});

		input_manager_->registerAction(PAL::InputManager::Up, [&]()
			{
				camera_.moveForward();
				renderer_->MoveCamera(camera_);
			});

		input_manager_->registerAction(PAL::InputManager::Left, [&]()
			{
				camera_.moveLeft();
				renderer_->MoveCamera(camera_);
			});

		input_manager_->registerAction(PAL::InputManager::Right, [&]()
			{
				camera_.moveRight();
				renderer_->MoveCamera(camera_);
			});

		//input_manager_->registerAxis(PAL::InputManager::VerticalLook, [&](float delta)
		//	{
		//		camera_.look_at_y += delta * -500;
		//		renderer_->MoveEye(camera_);
		//		recenter_cursor();
		//	});
		//
		//input_manager_->registerAxis(PAL::InputManager::HorizontalLook, [&](float delta)
		//	{
		//		auto const forward = calcForward(camera_);
		//		auto const up = Vector{ 0, 1, 0 };
		//		auto left = cross(forward, up);
		//
		//		left.x *= -1 * (delta > 0 ? -1 : 1);
		//		left.y *= -1 * (delta > 0 ? -1 : 1);
		//		left.z *= -1 * (delta > 0 ? -1 : 1);
		//
		//		camera_.look_at_x += left.x;
		//		camera_.look_at_y += left.y;
		//		camera_.look_at_z += left.z;
		//
		//		renderer_->MoveEye(camera_);
		//
		//		recenter_cursor();
		//	});


		while (msg.message != WM_QUIT)
		{


			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				input_manager_->process(msg);
			}

			input_manager_->update();
			renderer_->Render();
		}
	}

	void Window::Update(Models const& current_scene_models, UpdateFunc const&& updateFunc)
	{
		update_func_ = std::move(updateFunc);
		renderer_->SetModelsToRender(current_scene_models);
	}
}