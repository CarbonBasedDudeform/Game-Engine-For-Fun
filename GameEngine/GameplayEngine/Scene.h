#pragma once

namespace Gameplay
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		virtual void Update() = 0;
	};
}