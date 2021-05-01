#pragma once
namespace Graphics
{
	class ICamera
	{
	public:
		virtual ~ICamera() = default;
		virtual DirectX::XMVECTOR getEye() const = 0;
		virtual DirectX::XMVECTOR getLookAt() const = 0;
	};
}
