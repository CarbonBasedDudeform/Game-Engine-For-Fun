#include "Window.h"
#include "DX11Renderer.h"
#include <gainput\gainput.h>

enum Button
{
	Down,
	Up,
	Left,
	Right,
	VerticalLook,
	HorizontalLook
};


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
		switch(message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	Window::Window(std::string& title, size_t height, size_t width, RendererTypes renderType)
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
				a.z * b.x -a.x * b.z ,
				a.x*b.y - a.y * b.x
		};
	}

	Vector calcForward(const Graphics::IRenderer::Camera& camera)
	{
		auto forward = Vector{ camera.eye_x - camera.look_at_x
							 , camera.eye_y - camera.look_at_y
							 , camera.eye_z - camera.look_at_z };
		
		normalize(forward);

		return forward;
	}

	static int surpress_count = 0;
	void Window::recenter_cursor()
	{
		POINT mouse_pos{};
		GetCursorPos(&mouse_pos);
		RECT window_rect;
		GetWindowRect(window_handle_, &window_rect);
		SetCursorPos((window_rect.left + window_rect.right)/2, (window_rect.bottom + window_rect.top)/2);
		surpress_count = 0;
	}

	void Window::Loop() {
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		gainput::InputManager manager;
		
		manager.SetDisplaySize(1920,1080);
		const gainput::DeviceId keyboardId = manager.CreateDevice<gainput::InputDeviceKeyboard>();
		const gainput::DeviceId mouseId = manager.CreateDevice<gainput::InputDeviceMouse>();
		const gainput::DeviceId padId = manager.CreateDevice<gainput::InputDevicePad>();
		const gainput::DeviceId touchId = manager.CreateDevice<gainput::InputDeviceTouch>();

		//map.GetManager
		auto map = gainput::InputMap(manager);

		//keyboard
		map.MapBool(Down, keyboardId, gainput::KeyS);
		map.MapBool(Up, keyboardId, gainput::KeyW);
		map.MapBool(Left, keyboardId, gainput::KeyA);
		map.MapBool(Right, keyboardId, gainput::KeyD);
		map.MapFloat(VerticalLook, mouseId, gainput::MouseAxisY);
		map.MapFloat(HorizontalLook, mouseId, gainput::MouseAxisX);
		
		
		while (msg.message != WM_QUIT)
		{
			
			manager.Update();
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				manager.HandleMessage(msg);
			}
			
			{
				//update_func_();

				if (map.GetBool(Down))
				{
					//calc forward vector
					//flip it
					//move that way
					auto forward = calcForward(camera_);

					forward.x *= 0.01;
					forward.y *= 0.01;
					forward.z *= 0.01;
					
					camera_.eye_x += forward.x;
					camera_.eye_y += forward.y;
					camera_.eye_z += forward.z;

					camera_.look_at_x += forward.x;
					camera_.look_at_y += forward.y;
					camera_.look_at_z += forward.z;
					renderer_->MoveEye(camera_);
				}

				if (map.GetBool(Up))
				{
					//calc forward vector
					//move that way
					auto forward = calcForward(camera_);

					forward.x *= -1;
					forward.y *= -1;
					forward.z *= -1;
					
					forward.x *= 0.01;
					forward.y *= 0.01;
					forward.z *= 0.01;
					
					camera_.eye_x += forward.x;
					camera_.eye_y += forward.y;
					camera_.eye_z += forward.z;

					camera_.look_at_x += forward.x;
					camera_.look_at_y += forward.y;
					camera_.look_at_z += forward.z;
					renderer_->MoveEye(camera_);
				}

				if (map.GetBool(Left))
				{
					//calc forward vector
					//rotate it 90 around up vector.
					//move that way
					auto const forward = calcForward(camera_);
					auto const up = Vector{ 0, 1, 0 };
					auto left = cross(forward, up);

					left.x *= -0.01;
					left.y *= -0.01;
					left.z *= -0.01;
					
					camera_.eye_x += left.x;
					camera_.eye_y += left.y;
					camera_.eye_z += left.z;

					camera_.look_at_x += left.x;
					camera_.look_at_y += left.y;
					camera_.look_at_z += left.z;
					renderer_->MoveEye(camera_);
				}
				
				if (map.GetBool(Right))
				{
					auto const forward = calcForward(camera_);
					auto const up = Vector{ 0, 1, 0 };
					auto right = cross(forward, up);

					right.x *= 0.01;
					right.y *= 0.01;
					right.z *= 0.01;

					camera_.eye_x += right.x;
					camera_.eye_y += right.y;
					camera_.eye_z += right.z;

					camera_.look_at_x += right.x;
					camera_.look_at_y += right.y;
					camera_.look_at_z += right.z;
					renderer_->MoveEye(camera_);
				}


				if (surpress_count >= 50)
				{
					if (map.GetFloatDelta(VerticalLook) != 0.0)
					{
						auto const delta = map.GetFloatDelta(VerticalLook);
						camera_.look_at_y += delta * -500;
						//camera_.look_at_y = std::clamp(camera_.look_at_y, -10.0f, 10.0f);
						renderer_->MoveEye(camera_);
						//SetCursorPos(1920 / 2, 1080 / 2);
						recenter_cursor();
					}

					if (map.GetFloatDelta(HorizontalLook) != 0.0)
					{
						auto const delta = map.GetFloatDelta(HorizontalLook);
						//camera_.look_at_x += delta * 500;
						//
						auto const forward = calcForward(camera_);
						auto const up = Vector{ 0, 1, 0 };
						auto left = cross(forward, up);

						left.x *= -1 * (delta > 0 ? -1 : 1);
						left.y *= -1 * (delta > 0 ? -1 : 1);
						left.z *= -1 * (delta > 0 ? -1 : 1);

						camera_.look_at_x += left.x;
						camera_.look_at_y += left.y;
						camera_.look_at_z += left.z;
						//camera_.look_at_x = std::clamp(camera_.look_at_x, -10.0f, 10.0f);
						renderer_->MoveEye(camera_);

						//SetCursorPos(1920 / 2, 1080 / 2);
						recenter_cursor();
					}
				}else
				{
					surpress_count++;
				}

				renderer_->Render();
			}
		}
	}

	void Window::Update(Models const& current_scene_models, UpdateFunc const&& updateFunc)
	{
		update_func_ = std::move(updateFunc);
		renderer_->SetModelsToRender(current_scene_models);
	}
}