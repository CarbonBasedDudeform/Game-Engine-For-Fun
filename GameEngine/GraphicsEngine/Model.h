#pragma once

#pragma warning(push, 0) //disable warnings for external headers
#include <vector>
#include <string>
#include <filesystem>
#include <map>
#pragma warning(pop)

namespace Graphics
{
	struct Vertex
	{
		float x, y, z, u, v;
		int texutre_index;
	};
	
	using ImageData = unsigned char*;

	struct PNGTexture
	{
		int Width, Height, Comp;
		ImageData Data;
	};

	using Material = PNGTexture;

	using Materials = std::vector<Material>;

	struct Mesh
	{
		int id;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::shared_ptr<Material> texture;
	};

	using Meshes = std::vector<Mesh>;

	class Model
	{
	public:
		Model(std::filesystem::path const& filename);

		bool isOk() const;
		const Meshes& getMeshes() const;

	private:
		bool loaded_okay_{};
		Meshes meshes_;
		std::map<std::string, std::shared_ptr<Material>> materials_;
	};

	using Models = std::vector<Model>;
}
