#include "InputScheme.hpp"

#include <iomanip>

#include "Files/Json/Json.hpp"

namespace acid {
InputScheme::InputScheme(const std::filesystem::path &filename) :
	file(filename, std::make_unique<Json>()) {
	// Load this scheme from the file right away.
	file.Load();
	file.GetNode() >> *this;

	File testOutFile(filename, std::make_unique<Json>());
	testOutFile.GetNode() = *this;
	testOutFile.Write(NodeFormat::Beautified);
}

InputAxis *InputScheme::GetAxis(const std::string &name) {
	auto it = axes.find(name);
	if (it == axes.end()) {
		Log::Warning("InputAxis was not found in input scheme: ", std::quoted(name), '\n');
		it = axes.emplace(name, std::make_unique<InputAxis>()).first;
	}
	return it->second.get();
}

InputAxis *InputScheme::AddAxis(const std::string &name, std::unique_ptr<InputAxis> &&axis) {
	return axes.emplace(name, std::move(axis)).first->second.get();
}

void InputScheme::RemoveAxis(const std::string &name) {
	if (auto it = axes.find(name); it != axes.end())
		axes.erase(it);
}

InputButton *InputScheme::GetButton(const std::string &name) {
	auto it = buttons.find(name);
	if (it == buttons.end()) {
		Log::Warning("InputButton was not found in input scheme: ", std::quoted(name), '\n');
		it = buttons.emplace(name, std::make_unique<InputButton>()).first;
	}
	return it->second.get();
}

InputButton *InputScheme::AddButton(const std::string &name, std::unique_ptr<InputButton> &&button) {
	return buttons.emplace(name, std::move(button)).first->second.get();
}

void InputScheme::RemoveButton(const std::string &name) {
	if (auto it = buttons.find(name); it != buttons.end())
		buttons.erase(it);
}

const Node &operator>>(const Node &node, InputScheme &inputScheme) {
	node["axes"].Get(inputScheme.axes);
	node["buttons"].Get(inputScheme.buttons);
	return node;
}

Node &operator<<(Node &node, const InputScheme &inputScheme) {
	node["axes"].Set(inputScheme.axes);
	node["buttons"].Set(inputScheme.buttons);
	return node;
}

void InputScheme::MoveSignals(InputScheme *other) {
	if (!other) return;
	// Move all axis and button top level signals from the other scheme.
	for (auto &[axisName, axis] : other->axes) {
		if (auto it = axes.find(axisName); it != axes.end())
			std::swap(it->second->OnAxis(), axis->OnAxis());
		else
			Log::Warning("InputAxis was not found in input scheme: ", std::quoted(axisName), '\n');
	}
	for (auto &[buttonName, button] : other->buttons) {
		if (auto it = buttons.find(buttonName); it != buttons.end())
			std::swap(it->second->OnButton(), button->OnButton());
		else
			Log::Warning("InputButton was not found in input scheme: ", std::quoted(buttonName), '\n');
	}
}
}
