#pragma once

#pragma warning(push, 0) //disable warnings for external headers
#include <vector>
#include <string>
#include <filesystem>
#pragma warning(pop)

namespace Graphics
{
	struct Vertex
	{
		float x, y, z, u, v;
		//int texutre_index;
	};
	
	using ImageData = unsigned char*;

	struct Texture
	{
		int Width, Height, Comp;
		ImageData Data;
	};

	using Textures = std::vector<Texture>;

	class Model
	{
	public:
		Model(std::filesystem::path const& filename);

		bool isOk() const;
		std::vector<Vertex> getVertices() const;
		std::vector<unsigned int> getIndices() const;
		Textures getTextures() const;
	private:
		bool loaded_okay_{};
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		Textures textures_;
	};

	using Models = std::vector<Model>;
}
