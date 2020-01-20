#include "Model.h"

#pragma warning(push, 0) //disable warnings for external headers


#include <iostream> //TODO: replace with actual logging service...
//#define TINYOBJLOADER_IMPLEMENTATION
//#include "..\\..\\Externals\tinyobj_loader_opt.h"
//#include "..\\..\\Externals\OBJ_Loader.h"
#include "..\\..\\Externals\OBJ_Loader.h"
#pragma warning(pop)

namespace Graphics
{
	Model::Model(std::filesystem::path const& filename)
	{
		objl::Loader loader;
		loaded_okay_ = loader.LoadFile(filename.string());
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
		auto verts = loader.LoadedVertices;
		for (auto& v : verts)
		{
			vertices.emplace_back(Vertex{ v.Position.X, v.Position.Y, v.Position.Z });
		}
		
		indices = loader.LoadedIndices;
		
		//std::string warn;
		//std::string err;
		//
		//loaded_okay_ = tinyobj::LoadObj(&attribute_, &shapes_, &materials_, &warn, &err, filename.string().c_str());
		//
		//if (!warn.empty()) {
		//	std::cout << warn << std::endl;
		//}
		//
		//if (!err.empty()) {
		//	std::cerr << err << std::endl;
		//}
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