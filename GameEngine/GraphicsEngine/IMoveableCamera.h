#pragma once

namespace Graphics
{
	class IMoveableCamera
	{
		virtual void moveBack() = 0;
		virtual void moveForward() = 0;
		virtual void moveLeft() = 0;
		virtual void moveRight() = 0;
	};
}