#include "PointLightUI.h"
#include "ImGui/imgui.h"

namespace UI::Light::Point {
	void EditorPointLight::render() {

		ImGui::Begin(name.c_str());

		ImGui::ColorEdit4("Hue", light.hue);
		//ImGui::InputFloat3("Location", pl.point);
		
		ImGui::SliderFloat("X", &(light.point[0]), -100000.0f, 100000.0f);
		ImGui::SliderFloat("Y", &light.point[1], -100000.0f, 100000.0f);
		ImGui::SliderFloat("Z", &light.point[2], -100000.0f, 100000.0f);

		ImGui::End();
	}
};