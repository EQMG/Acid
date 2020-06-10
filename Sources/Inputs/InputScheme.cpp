#include "InputScheme.hpp"

#include <iomanip>

#include "Files/Json/Json.hpp"

namespace acid {
InputScheme::InputScheme(const std::filesystem::path &filename) :
	file(filename, std::make_unique<Json>()) {
	// Load this scheme from the file right away.
	file.Load();
	file.GetNode() >> *this;

	/*File argsFile("ArgumentDescriptionMap.json");
	std::map<std::string, InputAxis::ArgumentDescription> argumentDescriptionMap;
	for (const auto &[name, createFunc] : InputAxis::Registry())
		argumentDescriptionMap[name] = createFunc()->GetArgumentDescription();
	for (const auto &[name, createFunc] : InputButton::Registry())
		argumentDescriptionMap[name] = createFunc()->GetArgumentDescription();
	*argsFile.GetNode() << argumentDescriptionMap;
	argsFile.Write(NodeFormat::Beautified);*/

	File testOutFile(filename, std::make_unique<Json>());
	testOutFile.GetNode() = *this;
	testOutFile.Write(NodeFormat::Beautified);
}

InputAxis *InputScheme::GetAxis(const std::string &name) {
	auto it = axes.find(name);
	if (it == axes.end()) {
		Log::Error("InputAxis was not found in current input scheme: ", std::quoted(name), '\n');
		it = axes.emplace(name, std::make_unique<InputAxis>()).first;
	}
	return it->second.get();
}

InputAxis *InputScheme::AddAxis(const std::string &name, std::unique_ptr<InputAxis> &&axis) {
	return axes.emplace(name, std::move(axis)).first->second.get();
}

void InputScheme::RemoveAxis(const std::string &name) {
	auto it = axes.find(name);
	if (it != axes.end())
		axes.erase(it);
}

InputButton *InputScheme::GetButton(const std::string &name) {
	auto it = buttons.find(name);
	if (it == buttons.end()) {
		Log::Error("InputButton was not found in current input scheme: ", std::quoted(name), '\n');
		it = buttons.emplace(name, std::make_unique<InputButton>()).first;
	}
	return it->second.get();
}

InputButton *InputScheme::AddButton(const std::string &name, std::unique_ptr<InputButton> &&button) {
	return buttons.emplace(name, std::move(button)).first->second.get();
}

void InputScheme::RemoveButton(const std::string &name) {
	auto it = buttons.find(name);
	if (it != buttons.end())
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

void InputScheme::MoveDelegateOwnership(InputScheme *other) {
	if (!other) return;
	// Move all delegate functions except those owned internally by the axis or button.
	for (auto &[axisName, axis] : other->axes)
		GetAxis(axisName)->OnAxis().MoveFunctions(axis->OnAxis(), {axis->valid});
	for (auto &[buttonName, button] : other->buttons)
		GetButton(buttonName)->OnButton().MoveFunctions(button->OnButton(), {button->valid});
}
}
