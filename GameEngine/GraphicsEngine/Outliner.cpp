#include "Outliner.h"
#include "ImGui/imgui.h"
#include <vector>
#include <string>

#include "EditorObject.h"
#include "PointLightUI.h"

static bool exit() {
	std::exit(0);
	return true;
}

namespace UI::World::Outliner {
	void render() {
		static auto items = std::vector<std::unique_ptr<EditorObject>>();
		
		ImGui::BeginMainMenuBar();
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Exit", NULL)) {
					exit();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Entities")) {
				if (ImGui::MenuItem("Point Light", NULL)) {
					items.emplace_back(std::make_unique<UI::Light::Point::EditorPointLight>("Point Light" + std::to_string(items.size())));
				}
				ImGui::EndMenu();
			}
		ImGui::EndMainMenuBar();

		ImGui::Begin("Outliner");
		ImGui::BeginChild("World");
		
		for (auto& item : items) {
			if (ImGui::Button(item->name.c_str())) {
				item->display = !item->display;
			}
		}

		ImGui::EndChild();
		ImGui::End();

		for (auto& item : items) {
			if (item->display) {
				item->render();
			}
		}
	}
};