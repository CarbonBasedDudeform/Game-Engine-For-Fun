#pragma once

#pragma warning(push, 0) //disable warnings for external headers
#include <vector>
#include <string>
#include "..\\..\\Externals\tinyobj_loader_opt.h"
#pragma warning(pop)

namespace Graphics
{
	class Model
	{
	public:
		using Shape = tinyobj::shape_t;
		using Shapes = std::vector<Shape>;
		using Material = tinyobj::material_t;
		using Materials = std::vector<Material>;
		using Attribute = tinyobj::attrib_t;

		Model(std::string const& filename);

		bool isOk() const;
		Shapes getShapes() const;
		Materials getMaterials() const;
		Attribute getAttribute() const;

	private:
		bool loaded_okay_{};
		Shapes shapes_{};
		Materials materials_{};
		Attribute attribute_{};
	};

	using Models = std::vector<Model>;
}
