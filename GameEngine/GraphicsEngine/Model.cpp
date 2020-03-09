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
		auto const mtl_path = filename.parent_path().string();
		loaded_okay_ = tinyobj::LoadObj(&attribute_, &shapes_, &materials_, &warn, &err, filename.string().c_str(), mtl_path.c_str());

		for (auto& shape : shapes_) 
		{
			for (auto& i : shape.mesh.indices) 
			{
				int const texture_index = shape.mesh.material_ids[i.vertex_index];
				indices.push_back(indices.size());
				vertices.push_back(Vertex{ attribute_.vertices[3*i.vertex_index + 0], 
										   attribute_.vertices[3*i.vertex_index + 1], 
										   attribute_.vertices[3*i.vertex_index + 2],
										   attribute_.texcoords[2 * i.texcoord_index + 0],
										   attribute_.texcoords[2 * i.texcoord_index + 1]
										   //,texture_index
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

		for (auto& material : materials_)
		{
			bool const bad_material = material.diffuse_texname.empty();
			if (bad_material) continue;

			auto const texture_path = parent / std::filesystem::path{ material.diffuse_texname };
			bool const does_not_exist = !std::filesystem::exists(texture_path);
			if (does_not_exist) continue;

			auto texture = Texture{};
			texture.Data = stbi_load(texture_path.string().c_str(), &texture.Width, &texture.Height, &texture.Comp, STBI_rgb_alpha);
			textures_.push_back(texture);
		}
	}

	Textures Model::getTextures() const
	{
		return textures_;
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