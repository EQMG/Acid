#include "InputScheme.hpp"

#include <iomanip>

namespace acid {
InputScheme::InputScheme(const std::filesystem::path &filename) :
	file(filename) {
	// Load this scheme from the file right away.
	file.Load();
	*file.GetNode() >> *this;

	/*File argsFile("ArgumentDescriptionMap.json");
	std::map<std::string, Axis::ArgumentDescription> argumentDescriptionMap;
	for (const auto &[name, createFunc] : Axis::Registry())
		argumentDescriptionMap[name] = createFunc()->GetArgumentDescription();
	for (const auto &[name, createFunc] : Button::Registry())
		argumentDescriptionMap[name] = createFunc()->GetArgumentDescription();
	*argsFile.GetNode() << argumentDescriptionMap;
	argsFile.Write(Node::Format::Beautified);*/

	File testOutFile(filename);
	*testOutFile.GetNode() << *this;
	testOutFile.Write(Node::Format::Beautified);
}

Axis *InputScheme::GetAxis(const std::string &name) {
	auto it = axes.find(name);
	if (it == axes.end()) {
		Log::Error("Axis was not found in current input scheme: ", std::quoted(name), '\n');
		it = axes.emplace(name, std::make_unique<Axis>()).first;
	}
	return it->second.get();
}

Axis *InputScheme::AddAxis(const std::string &name, std::unique_ptr<Axis> &&axis) {
	return axes.emplace(name, std::move(axis)).first->second.get();
}

void InputScheme::RemoveAxis(const std::string &name) {
	auto it = axes.find(name);
	if (it != axes.end())
		axes.erase(it);
}

Button *InputScheme::GetButton(const std::string &name) {
	auto it = buttons.find(name);
	if (it == buttons.end()) {
		Log::Error("Button was not found in current input scheme: ", std::quoted(name), '\n');
		it = buttons.emplace(name, std::make_unique<Button>()).first;
	}
	return it->second.get();
}

Button *InputScheme::AddButton(const std::string &name, std::unique_ptr<Button> &&button) {
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
