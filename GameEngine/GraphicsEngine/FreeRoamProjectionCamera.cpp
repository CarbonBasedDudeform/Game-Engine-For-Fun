#include "FreeRoamProjectionCamera.h"

namespace 
{
	DirectX::XMVECTOR calcForward(const Graphics::ICamera& camera)
	{
		auto forward = DirectX::XMVectorSubtract(camera.getEye(), camera.getLookAt());
		auto normalized_forward = DirectX::XMVector3Normalize(forward);
		return normalized_forward;
	}
}

namespace  Graphics
{
	FreeRoamProjectionCamera::FreeRoamProjectionCamera(float x, float y, float z)
	{
		eye_ = DirectX::XMVectorSet(x, y, z, 0);
	}

	DirectX::XMVECTOR FreeRoamProjectionCamera::getEye() const
	{
		return eye_;
	}

	DirectX::XMVECTOR FreeRoamProjectionCamera::getLookAt() const
	{
		return look_at_;
	}

	void FreeRoamProjectionCamera::moveBack()
	{
		//calc forward vector
		//move that way

		auto forward = calcForward(*this);
		forward = DirectX::XMVectorScale(forward, move_speed_);
		eye_ = DirectX::XMVectorAdd(eye_, forward);
		look_at_ = DirectX::XMVectorAdd(look_at_, forward);
	}

	void FreeRoamProjectionCamera::moveForward()
	{
		//calc forward vector
		//flip it
		//move that way
		auto forward = calcForward(*this);
		forward = DirectX::XMVectorScale(forward, -move_speed_);
		eye_ = DirectX::XMVectorAdd(eye_, forward);
		look_at_ = DirectX::XMVectorAdd(look_at_, forward);
	}
	
	void FreeRoamProjectionCamera::moveLeft()
	{
		//calc forward vector
						//rotate it 90 around up vector.
						//move that way
		
		auto const forward = calcForward(*this);
		auto const up = DirectX::XMVectorSet( 0, 1, 0, 0);
		auto left = DirectX::XMVector3Cross(forward, up);
		left = DirectX::XMVectorScale(left, -move_speed_);
		eye_ = DirectX::XMVectorAdd(eye_, left);
		look_at_ = DirectX::XMVectorAdd(look_at_, left);
	}
	
	void FreeRoamProjectionCamera::moveRight()
	{
		auto const forward = calcForward(*this);
		auto const up = DirectX::XMVectorSet(0, 1, 0, 0);
		auto left = DirectX::XMVector3Cross(forward, up);
		left = DirectX::XMVectorScale(left, move_speed_);
		eye_ = DirectX::XMVectorAdd(eye_, left);
		look_at_ = DirectX::XMVectorAdd(look_at_, left);
	}

	void FreeRoamProjectionCamera::rotatePitch(float delta)
	{
		auto adjustment = DirectX::XMVectorSet(0, (delta > 0 ? 0.1 : -0.1), 0, 0);
		look_at_ = DirectX::XMVectorAdd(look_at_, adjustment);
	}

	void FreeRoamProjectionCamera::rotateYaw(float delta)
	{
		auto const forward = calcForward(*this);
		auto const up = DirectX::XMVectorSet(0, 1, 0, 0);
		auto left = DirectX::XMVector3Cross(forward, up);
		left = DirectX::XMVectorScale(left, (delta > 0 ? 0.1 : -0.1));
		look_at_ = DirectX::XMVectorAdd(look_at_, left);
	}
}
