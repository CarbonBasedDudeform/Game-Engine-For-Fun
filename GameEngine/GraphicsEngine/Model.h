#pragma once

#pragma warning(push, 0) //disable warnings for external headers
#include <vector>
#include <string>
//#include "..\\..\\Externals\tinyobj_loader_opt.h"
//#include "..\\..\\Externals\OBJ_Loader.h"
#include <filesystem>
#pragma warning(pop)

//namespace objl
//{
//	//struct Vertex;
//	class Loader;
//	struct Mesh;
//}

namespace Graphics
{
	struct Vertex
	{
		float x, y, z;
	};

	class Model
	{
	public:
		//using Shape = tinyobj::shape_t;
		//using Shapes = std::vector<Shape>;
		//using Material = tinyobj::material_t;
		//using Materials = std::vector<Material>;
		//using Attribute = tinyobj::attrib_t;
		
		Model(std::filesystem::path const& filename);

		bool isOk() const;
		//Shapes getShapes() const;
		//Materials getMaterials() const;
		//Attribute getAttribute() const;
		//std::vector<objl::Mesh> getMesh() const;
		std::vector<Vertex> getVertices() const;
		std::vector<unsigned int> getIndices() const;
	private:
		bool loaded_okay_{};
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		//objl::Loader loader;
		//Shapes shapes_{};
		//Materials materials_{};
		//Attribute attribute_{};
	};

	using Models = std::vector<Model>;
}
