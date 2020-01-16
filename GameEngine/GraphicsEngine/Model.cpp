#include "Model.h"

#pragma warning(push, 0) //disable warnings for external headers


#include <iostream> //TODO: replace with actual logging service...
#define TINYOBJLOADER_IMPLEMENTATION
#include "..\\..\\Externals\tinyobj_loader_opt.h"

#pragma warning(pop)

namespace Graphics
{
	Model::Model(std::filesystem::path const& filename)
	{
		std::string warn;
		std::string err;

		loaded_okay_ = tinyobj::LoadObj(&attribute_, &shapes_, &materials_, &warn, &err, filename.string().c_str());

		if (!warn.empty()) {
			std::cout << warn << std::endl;
		}

		if (!err.empty()) {
			std::cerr << err << std::endl;
		}
	}

	bool Model::isOk() const
	{
		return loaded_okay_;
	}

	Model::Shapes Model::getShapes() const
	{
		return shapes_;
	}

	Model::Materials Model::getMaterials() const
	{
		return materials_;
	}

	Model::Attribute Model::getAttribute() const
	{
		return attribute_;
	}
}