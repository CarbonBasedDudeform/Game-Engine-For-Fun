#include "Outliner.h"
#include "ImGui/imgui.h"
#include <vector>
#include <string>

struct EditorObject {
	std::string name{};
};

static bool exit() {
	std::exit(0);
	return true;
}

namespace UI::World::Outliner {
	void render() {
		bool wants_exit = false;
		ImGui::BeginMainMenuBar();
			if (ImGui::BeginMenu("File")) {
				ImGui::MenuItem("Exit", NULL, &wants_exit);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Entities")) {
				ImGui::MenuItem("Point Light", NULL, &wants_exit);
				ImGui::EndMenu();
			}
		ImGui::EndMainMenuBar();

		ImGui::Begin("Outliner");
		static auto current_item = 0;
		static auto items = std::vector<EditorObject>();
		ImGui::BeginChild("World");
		for (auto item : items) {
			ImGui::Button(item.name.c_str());
		}
		ImGui::EndChild();
		ImGui::End();

		if (wants_exit) {
			exit();
		}
	}
};