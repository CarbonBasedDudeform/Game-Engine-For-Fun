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
	};
	
	using ImageData = unsigned char*;

	struct Texture
	{
		int Width, Height, Comp;
		ImageData Data;
	};

	class Model
	{
	public:
		Model(std::filesystem::path const& filename);

		bool isOk() const;
		std::vector<Vertex> getVertices() const;
		std::vector<unsigned int> getIndices() const;
		Texture getTexture() const;
	private:
		bool loaded_okay_{};
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		Texture texture_;
	};

	using Models = std::vector<Model>;
}
