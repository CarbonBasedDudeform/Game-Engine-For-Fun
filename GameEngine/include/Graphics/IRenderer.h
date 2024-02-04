#pragma once
#include <Graphics/Model.h>
#include <Graphics/ICamera.h>

#include <functional>

namespace Graphics
{
	enum RendererTypes
	{
		DX11,
		Metal
	};

	using RenderFunc = std::function<void()>;
	using UpdateFunc = std::function<void()>;

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;
		virtual bool create_context(size_t height, size_t width) = 0;
		virtual void render() = 0;
		virtual void move_camera(const ICamera& pos) = 0;

		void set_models_to_render(Models const& models) {
			scene_models = models;
		}

	protected:
		virtual void pre_frame_render_hook() = 0;
		virtual void post_frame_render_hook() = 0;
	
	protected:
		Models scene_models{};
	};
}