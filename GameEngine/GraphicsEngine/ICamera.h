#pragma once
namespace Graphics
{
	class ICamera
	{
	public:
		virtual DirectX::XMVECTOR getEye() const = 0;
		virtual DirectX::XMVECTOR getLookAt() const = 0;
	};
}
