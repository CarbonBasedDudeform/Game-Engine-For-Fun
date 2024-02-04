#pragma once
#include <Math/Vector.h>

namespace Graphics
{
	class ICamera
	{
	public:
		virtual ~ICamera() = default;
		virtual Vector getEye() const = 0;
		virtual Vector getLookAt() const = 0;
	};
}
