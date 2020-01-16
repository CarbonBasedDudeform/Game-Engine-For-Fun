#pragma once
#include <filesystem>

#include "GameplayDataStructs.h"
#include "..\GraphicsEngine\Model.h"

namespace Gameplay
{
	class Scene
	{
	public:
		

		Scene();
		virtual ~Scene();
		virtual void RenderFrame() = 0;
		virtual void Update(Gameplay::GameTimeDelta timeDelta) = 0;

		bool AddModel(std::filesystem::path const& path_to_model);
		Graphics::Models getModels() const;

	private:
		Graphics::Models models_{};
	};
}