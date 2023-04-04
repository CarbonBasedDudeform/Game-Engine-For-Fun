#pragma once
#include "IRenderer.h"

struct Vector3D {
	float x{};
	float y{};
	float z{};
};

using Hue = float[4];
using Point = float[4];

struct PointLight {
	Hue hue{};
	Point point{};
	float intensity{};
};

namespace UI::Light::Point {
	static PointLight light{};
	void render();
};