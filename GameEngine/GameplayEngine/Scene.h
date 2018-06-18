#pragma once
#include "GameplayDataStructs.h"

namespace Gameplay
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();
		virtual void RenderFrame() = 0;
		virtual void Update(Gameplay::GameTimeDelta timeDelta) = 0;
	};
}