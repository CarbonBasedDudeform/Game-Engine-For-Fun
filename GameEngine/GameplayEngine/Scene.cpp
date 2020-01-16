#include "Scene.h"

namespace Gameplay
{
	Scene::Scene()
	{
	}


	Scene::~Scene()
	{
	}

	bool Scene::AddModel(std::filesystem::path const& path_to_model)
	{
		auto new_model = Graphics::Model{ path_to_model };
		if (!new_model.isOk())
		{
			return false;
		}

		models_.emplace_back(new_model);
		
		return true;
	}

	Graphics::Models Scene::getModels() const
	{
		return models_;
	}
}