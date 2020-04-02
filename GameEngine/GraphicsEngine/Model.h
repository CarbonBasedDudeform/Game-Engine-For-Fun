#pragma once

#pragma warning(push, 0) //disable warnings for external headers
#include <vector>
#include <string>
#include <filesystem>
#include <map>
#include <DirectXMath.h>
#pragma warning(pop)

namespace Graphics
{
	struct Vector {
		float x, y, z;
	};
	
	struct Point {
		float x, y, z, w;
	};

	struct Rotation {
		Point x,y,z,w;
	};

	//using Rotation = std::array<std::array<float, 4>, 4>;

	Rotation makeRotationMatrixUsingRadians(float angle) noexcept;

	

	struct ConstantBuffer {
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	struct Vertex
	{
		float x, y, z, u, v;
	};
	
	using ImageData = unsigned char*;

	struct PNGTexture
	{
		int Id, Width, Height, Comp;
		ImageData Data;
	};

	using Material = PNGTexture;

	using Materials = std::vector<std::string>;

	struct Mesh
	{
		int id;
		//std::vector<Vertex> vertices;
		int start, size;
		//std::vector<unsigned int> indices;
		std::map<std::string, std::vector<Vertex>> texture_verts_bucket;
		std::map<std::string, std::vector<unsigned int>> texture_idx_bucket;
		//std::shared_ptr<Material> texture;
	};

	using Meshes = std::vector<Mesh>;

	class Model
	{
	public:
		Model(std::filesystem::path const& filename);

		bool isOk() const;
		const Meshes& getMeshes() const;
		ConstantBuffer constant_buffer{};
		std::shared_ptr<Material> getTexture(const std::string& name);
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		Materials getMaterials() const;

	private:
		bool loaded_okay_{};
		Meshes meshes_;
		std::map<std::string, std::shared_ptr<Material>> materials_;
	};

	using Models = std::vector<Model>;
}
