#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <map>
//#include <DirectXMath.h>

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

	// struct ConstantBuffer
	// {
	// 	DirectX::XMMATRIX world;
	// 	DirectX::XMMATRIX view;
	// 	DirectX::XMMATRIX projection;
	// };

	// struct PointLightConstantBuffer
	// {
	// 	DirectX::XMMATRIX projection;
	// 	DirectX::XMMATRIX view;
	// 	DirectX::XMMATRIX world;
	// 	DirectX::XMFLOAT3 lightPosition;
	// 	DirectX::XMVECTOR eyePosition;
	// 	float intensity;
	// };

	struct Vertex
	{
		float x, y, z, n1, n2, n3, u, v;
	};
	
	using ImageData = unsigned char*;

	struct PNG
	{
		int Id, Width, Height, Comp;
		ImageData Data;
	};

	using Image = PNG;

	using MaterialNames = std::vector<std::string>;

	class Model
	{
	public:
		Model(std::filesystem::path const& filename);

		bool isOk() const;
		//PointLightConstantBuffer constant_buffer{};
		std::shared_ptr<Image> getImage(const std::string& name);
		MaterialNames getMaterialNames() const;
		std::map<std::string, std::vector<Vertex>> texture_verts_bucket;
	private:
		bool loaded_okay_{};
		std::map<std::string, std::shared_ptr<Image>> materials_;
	};

	using Models = std::vector<Model>;
}
