#include "Model.h"

#pragma warning(push, 0) //disable warnings for external headers


#include <iostream> //TODO: replace with actual logging service...
#define TINYOBJLOADER_IMPLEMENTATION
#include "..\\..\\Externals\tinyobj_loader_opt.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\\..\\Externals\stb_image.h"
#pragma warning(pop)

namespace Graphics
{
	Model::Model(std::filesystem::path const& filename)
	{	
		std::string warn;
		std::string err;
		tinyobj::attrib_t attribute_;
		std::vector<tinyobj::shape_t> shapes_;
		std::vector<tinyobj::material_t> materials_;

		loaded_okay_ = tinyobj::LoadObj(&attribute_, &shapes_, &materials_, &warn, &err, filename.string().c_str());

		for (auto& shape : shapes_) 
		{
			for (auto& i : shape.mesh.indices) 
			{
				indices.push_back(indices.size());
				vertices.push_back(Vertex{ attribute_.vertices[3*i.vertex_index + 0], 
										   attribute_.vertices[3*i.vertex_index + 1], 
										   attribute_.vertices[3*i.vertex_index + 2],
										   attribute_.texcoords[2 * i.texcoord_index + 0],
										   attribute_.texcoords[2 * i.texcoord_index + 1] 
				});
			}
		}
		
		if (!warn.empty()) {
			std::cout << warn << std::endl;
		}
		
		if (!err.empty()) {
			std::cerr << err << std::endl;
		}

		auto const stem = filename.stem();
		auto const parent = filename.parent_path();
		auto const texture_path = parent / std::filesystem::path{ stem.string() + std::string{".png"}};
		texture_.Data = stbi_load(texture_path.string().c_str(), &texture_.Width, &texture_.Height, &texture_.Comp, STBI_rgb_alpha);
	}

	Texture Model::getTexture() const
	{
		return texture_;
	}

	bool Model::isOk() const
	{
		return loaded_okay_;
	}

	std::vector<Vertex> Model::getVertices() const
	{
		return vertices;
	}


	std::vector<unsigned int> Model::getIndices() const
	{
		return indices;
	}
}