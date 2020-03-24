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
		tinyobj::attrib_t attribute;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		auto const mtl_path = filename.parent_path().string();
		loaded_okay_ = tinyobj::LoadObj(&attribute, &shapes, &materials, &warn, &err, filename.string().c_str(), mtl_path.c_str());
		
		auto const parent = filename.parent_path();

		for (auto& material : materials)
		{
			auto const texture_path = parent / std::filesystem::path{ material.diffuse_texname };
			auto texture = std::make_shared<Material>();
			texture->Data = stbi_load(texture_path.string().c_str(), &texture->Width, &texture->Height, &texture->Comp, STBI_rgb_alpha);
			materials_[material.name] = std::move(texture);
		}

		int count = 0;
		for (auto& shape : shapes)
		{
			auto mesh = Mesh{};
			mesh.id = count++;

			for (auto& i : shape.mesh.indices) 
			{
				//attribute.vertices[3 * i.vertex_index + 2];
				mesh.indices.push_back(mesh.indices.size());
				bool const texture_coords_exist = i.texcoord_index != -1;
				mesh.vertices.push_back(Vertex{ attribute.vertices[3*i.vertex_index + 0], 
												attribute.vertices[3*i.vertex_index + 1], 
												attribute.vertices[3*i.vertex_index + 2],
												texture_coords_exist ? attribute.texcoords[2 * i.texcoord_index + 0] : 0,
												texture_coords_exist ? attribute.texcoords[2 * i.texcoord_index + 1] : 0
				});

				int const texture_index = shape.mesh.material_ids[0]; //...not ideal
				bool const textures_arent_used = texture_index < 0;
				if (textures_arent_used) continue;

				auto const material_name = materials[texture_index].name;
				bool const material_doesnt_exist = materials_.find(material_name) == materials_.end();
				if (material_doesnt_exist) throw std::exception{"Trying to use non-existant material"};
				
				mesh.texture = materials_[material_name];
			}

			meshes_.push_back(mesh);
		}

		
		if (!warn.empty()) {
			std::cout << warn << std::endl;
		}
		
		if (!err.empty()) {
			std::cerr << err << std::endl;
		}

		//constant_buffer.rotation = makeRotationMatrixUsingRadians(3.14/3.0 * 2);
	}

	bool Model::isOk() const
	{
		return loaded_okay_;
	}

	const Meshes& Model::getMeshes() const
	{
		return meshes_;
	}

	Rotation makeRotationMatrixUsingRadians(float amount) noexcept
	{
			return {
				{ cos(amount),0, -sin(amount), 0},
				{ 0         ,1,           0, 0},
				{sin(amount) ,0,  cos(amount), 0},
				{ 0         ,0,           0, 1}
			};
	}
}