#pragma once
#include "IRenderer.h"
#include "EditorObject.h"

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
	struct EditorPointLight : public EditorObject {
		EditorPointLight(const std::string& name) : EditorObject(name) {};

		PointLight light{};
		virtual void render() override;
	};
};