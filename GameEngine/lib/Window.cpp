
#include <SDL.h>
#include <Graphics/Window.h>
#include <Graphics/Model.h>
#include <Graphics/IRenderer.h>

//#include "windowsx.h"
//#include "DX11Renderer.h"
//#include "IMGUIManager.h"

//#include "ImGui/Backends/imgui_impl_win32.h"

// namespace PAL
// {
// 	class InputManager
// 	{
// 	public:
// 		using Pressable = int;
// 		enum Button
// 		{
// 			VerticalLook,
// 			HorizontalLook
// 		};

// 		using Action = std::function<void()>;
// 		using Axis = std::function<void(float)>;
		
		
// 		void registerAction(Pressable button, const Action&& action);
// 		void registerAxis(Button button, const Axis&& axis);
// 		void process(const MSG& msg);
// 		void SetUserInterface(const std::shared_ptr<Graphics::UI::IUserInterfaceManager>& user_interface) {
// 			user_interface_ = user_interface;
// 		}

// 	private:
// 		int last_X_{};
// 		int last_Y_{};

// 		std::unordered_map<Pressable, Action> action_map_{};
// 		std::unordered_map<Button, Axis> axis_map_{};
// 		std::shared_ptr<Graphics::UI::IUserInterfaceManager> user_interface_{};
// 	};
// }


// void PAL::InputManager::process(const MSG& msg)
// {
// 	if (user_interface_->ProcessInput())
// 	{
// 		return;
// 	}

// 	if (msg.message == WM_MOUSEMOVE)
// 	{
// 		int x = GET_X_LPARAM(msg.lParam);
// 		int y = GET_Y_LPARAM(msg.lParam);

// 		if (x != last_X_)
// 		{
// 			int delta = x - last_X_;
// 			axis_map_[Button::HorizontalLook](static_cast<float>(delta));
// 			last_X_ = x;
// 		}

// 		if (y != last_Y_)
// 		{
// 			int delta = y - last_Y_;
// 			axis_map_[Button::VerticalLook](static_cast<float>(delta));
// 			last_Y_ = y;
// 		}
// 	}
// 	else if (msg.message == WM_KEYDOWN)
// 	{
// 		if (action_map_.contains(msg.wParam))
// 		{
// 			action_map_[msg.wParam]();
// 		}
// 	}
// }

// void PAL::InputManager::registerAction(Pressable button, const Action&& action)
// {
// 	action_map_[button] = action;
// }

// void PAL::InputManager::registerAxis(Button button, const Axis&& axis)
// {
// 	axis_map_[button] = axis;
// }

//IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Graphics
{
	std::function<void(SDL_Window*)> Window::sdl_deleter = [](SDL_Window* w) {
		SDL_DestroyWindow(w);
	};

	void Window::create(std::string const& title, size_t height, size_t width)
	{
		SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

		auto sdlDeleter = [](SDL_Window* w) {
    		SDL_DestroyWindow(w);
		};

		// Create an SDL window
		window.reset(SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_SHOWN));

		// WNDCLASSEX window;
		// ZeroMemory(&window, sizeof(window));
		// window.cbSize = sizeof(WNDCLASSEX);
		// window.style = CS_CLASSDC;
		// window.lpfnWndProc = &WndProc;
		// window.hInstance = instance_;
		// window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		// window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		// window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		// window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		// window.lpszClassName = title.c_str();

		// RegisterClassEx(&window);
		// window_handle_ = CreateWindow(title.c_str(), title.c_str(), WS_BORDER, 0, 0, static_cast<int>(width), static_cast<int>(height), nullptr, nullptr, instance_, nullptr);

		// user_interface_->Initialise(width, height, window_handle_);

		// ShowWindow(window_handle_, SW_SHOW);
		// UpdateWindow(window_handle_);

		// RECT clientRect;
		// GetClientRect(window_handle_, &clientRect);

		// renderer_->CreateContext(clientRect.bottom, clientRect.right, window_handle_);

		//ShowCursor(false);

	}


	Window::Window(const std::string& title, size_t height, size_t width)
		//: input_manager_(std::make_shared<PAL::InputManager>())
		//, user_interface_(std::make_shared<UI::IMGUIManager>())
	{
		//switch (renderType)
		//{
		//case RendererTypes::DX11:
		//	renderer = std::make_unique<DX11Renderer>();
		//}
		create(title, height, width);
	}

	Window::~Window()
	{
    	SDL_Quit();
	}
	
	void Window::loop() {
		// MSG msg;
		// ZeroMemory(&msg, sizeof(msg));

		// input_manager_->registerAction(VK_ESCAPE, [&]() noexcept
		// 	{
		// 		std::quick_exit(0);
		// 	});

		// input_manager_->registerAction('S', [&]()
		// 	{
		// 		camera_.moveBack();
		// 		renderer_->MoveCamera(camera_);
		// 	});

		// input_manager_->registerAction('W', [&]()
		// 	{
		// 		camera_.moveForward();
		// 		renderer_->MoveCamera(camera_);
		// 	});

		// input_manager_->registerAction('A', [&]()
		// 	{
		// 		camera_.moveLeft();
		// 		renderer_->MoveCamera(camera_);
		// 	});

		// input_manager_->registerAction('D', [&]()
		// 	{
		// 		camera_.moveRight();
		// 		renderer_->MoveCamera(camera_);
		// 	});

		// input_manager_->registerAxis(PAL::InputManager::VerticalLook, [&](float delta)
		// 	{
		// 		camera_.rotatePitch(delta);
		// 		renderer_->MoveCamera(camera_);
		// 	});

		// input_manager_->registerAxis(PAL::InputManager::HorizontalLook, [&](float delta)
		// 	{
		// 		camera_.rotateYaw(delta);
		// 		renderer_->MoveCamera(camera_);
		// 	});

		// input_manager_->SetUserInterface(user_interface_);

		// while (msg.message != WM_QUIT)
		// {
		// 	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		// 	{
		// 		TranslateMessage(&msg);
		// 		DispatchMessage(&msg);

		// 		input_manager_->process(msg);
		// 	}

		// 	user_interface_->Render();
		// 	renderer_->Render();
		// }

		bool running = true;
    	while (running) {
    	    SDL_Event event;
    	    while (SDL_PollEvent(&event)) {
    	        if (event.type == SDL_QUIT) {
    	            running = false;
    	        }
    	    }
    	}
	}

	void Window::set_scene(Models const& current_scene_models)
	{
		if (!renderer) return;

		//update_func = updateFunc;
		renderer->set_models_to_render(current_scene_models);
	}
}