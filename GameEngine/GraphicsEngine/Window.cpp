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

	void Window::Loop() {
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		gainput::InputManager manager;
		
		manager.SetDisplaySize(800,600);
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
					camera_.eye_z-= 0.01f;
					camera_.look_at_z -= 0.01f;
					renderer_->MoveEye(camera_);
				}

				if (map.GetBool(Up))
				{
					camera_.eye_z += 0.01f;
					camera_.look_at_z += 0.01f;
					renderer_->MoveEye(camera_);
				}

				if (map.GetBool(Left))
				{
					camera_.look_at_x -= 0.01f;
					camera_.eye_x -= 0.01f;
					renderer_->MoveEye(camera_);
				}
				
				if (map.GetBool(Right))
				{
					camera_.look_at_x += 0.01f;
					camera_.eye_x += 0.01f;
					renderer_->MoveEye(camera_);
				}


				if (map.GetFloat(VerticalLook))
				{
					camera_.look_at_y += map.GetFloatDelta(VerticalLook) * 100;
					renderer_->MoveEye(camera_);
				}

				if (map.GetFloat(HorizontalLook))
				{
					camera_.look_at_x += map.GetFloatDelta(HorizontalLook) * 100;
					renderer_->MoveEye(camera_);
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