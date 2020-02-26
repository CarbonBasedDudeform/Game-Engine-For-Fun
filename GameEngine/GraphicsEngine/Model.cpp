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
		//objl::Loader loader;
		//loaded_okay_ = loader.LoadFile(filename.string());
		//for (auto& mesh : loader.LoadedMeshes)
		//{
		//	for (auto& v : mesh.Vertices)
		//	{
		//		vertices.emplace_back(Vertex{ v.Position.X, v.Position.Y, v.Position.Z });
		//	}
		//
		//	for (auto& i : mesh.Indices)
		//	{
		//		indices.emplace_back(i);
		//	}
		//}
		//auto verts = loader.LoadedVertices;
		//for (auto& v : verts)
		//{
		//	vertices.emplace_back(Vertex{ v.Position.X, v.Position.Y, v.Position.Z });
		//}
		//
		//indices = loader.LoadedIndices;
		
		std::string warn;
		std::string err;
		tinyobj::attrib_t attribute_;
		std::vector<tinyobj::shape_t> shapes_;
		std::vector<tinyobj::material_t> materials_;

		loaded_okay_ = tinyobj::LoadObj(&attribute_, &shapes_, &materials_, &warn, &err, filename.string().c_str());
		//for (int i = 1; i < attribute_.texcoords.size(); i++) 
		//{
		//	UVs.push_back({ attribute_.texcoords[i - 1], attribute_.texcoords[i] });
		//}

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

		//for (int i = 0; i < attribute_.vertices.size() - 3; i++)
		//{
		//	vertices.push_back(Vertex{ attribute_.vertices[i], attribute_.vertices[i + 1], attribute_.vertices[i + 2] });
		//}
		
		if (!warn.empty()) {
			std::cout << warn << std::endl;
		}
		
		if (!err.empty()) {
			std::cerr << err << std::endl;
		}

		auto const stem = filename.stem();
		auto const parent = filename.parent_path();
		auto const texture_path = parent / std::filesystem::path{ std::string{"default.png"}};
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

	//std::vector<objl::Mesh> Model::getMesh() const
	//{
	//	return loader.LoadedMeshes;
	//}

	//Model::Shapes Model::getShapes() const
	//{
	//	return shapes_;
	//}
	//
	//Model::Materials Model::getMaterials() const
	//{
	//	return materials_;
	//}
	//
	//Model::Attribute Model::getAttribute() const
	//{
	//	return attribute_;
	//}
}