#pragma once
#include <string>

struct EditorObject {
	EditorObject(const std::string& in_name) : name(in_name) {};
	
	std::string name{};
	bool display{ false };

	virtual void render() {};
};