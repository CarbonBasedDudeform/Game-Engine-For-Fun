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
		ConstantBuffer constant_buffer{};

	private:
		bool loaded_okay_{};
		Meshes meshes_;
		std::map<std::string, std::shared_ptr<Material>> materials_;
	};

	using Models = std::vector<Model>;
}
