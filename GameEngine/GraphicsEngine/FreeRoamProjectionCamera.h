#pragma once
#include <DirectXMath.h>

#include "ICamera.h"
#include "IMoveableCamera.h"

namespace Graphics
{
	class FreeRoamProjectionCamera :
		public ICamera, public IMoveableCamera
	{
	public:
		FreeRoamProjectionCamera(float x, float y, float z);
		
		DirectX::XMVECTOR getEye() const override;
		DirectX::XMVECTOR getLookAt() const override;

		void moveBack() override;
		void moveForward() override;
		void moveLeft() override;
		void moveRight() override;
		void rotatePitch(float delta) override;
		void rotateYaw(float delta) override;

	private:
		DirectX::XMVECTOR eye_;
		DirectX::XMVECTOR look_at_{};
	};
}
