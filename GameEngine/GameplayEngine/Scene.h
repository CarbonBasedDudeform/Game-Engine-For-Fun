#pragma once

namespace Gameplay
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();
		virtual void RenderFrame() = 0;
	};
}