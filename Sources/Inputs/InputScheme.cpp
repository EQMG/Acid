#include "InputScheme.hpp"

namespace acid {
InputScheme::InputScheme(const std::filesystem::path &filename) :
	m_file(filename) {
	// Load this scheme from the file right away.
	m_file.Load();
	*m_file.GetNode() >> *this;

	/*File argsFile("ArgumentDescriptionMap.json");
	std::map<std::string, Axis::ArgumentDescription> argumentDescriptionMap;
	for (const auto &[name, createFunc] : Axis::Registry())
		argumentDescriptionMap[name] = createFunc()->GetArgumentDescription();
	for (const auto &[name, createFunc] : Button::Registry())
		argumentDescriptionMap[name] = createFunc()->GetArgumentDescription();
	*argsFile.GetNode() << argumentDescriptionMap;
	argsFile.Write(Node::Format::Beautified);*/

	/*File testOutFile(filename);
	*testOutFile.GetNode() << *this;
	testOutFile.Write(Node::Format::Beautified);*/
}

Axis *InputScheme::GetAxis(const std::string &name) {
	auto it = m_axes.find(name);
	if (it == m_axes.end()) {
		Log::Error("Axis was not found in current input scheme: ", std::quoted(name), '\n');
		it = m_axes.emplace(name, std::make_unique<Axis>()).first;
	}
	return it->second.get();
}

Axis *InputScheme::AddAxis(const std::string &name, std::unique_ptr<Axis> &&axis) {
	return m_axes.emplace(name, std::move(axis)).first->second.get();
}

void InputScheme::RemoveAxis(const std::string &name) {
	auto it = m_axes.find(name);
	if (it != m_axes.end())
		m_axes.erase(it);
}

Button *InputScheme::GetButton(const std::string &name) {
	auto it = m_buttons.find(name);
	if (it == m_buttons.end()) {
		Log::Error("Button was not found in current input scheme: ", std::quoted(name), '\n');
		it = m_buttons.emplace(name, std::make_unique<Button>()).first;
	}
	return it->second.get();
}

Button *InputScheme::AddButton(const std::string &name, std::unique_ptr<Button> &&button) {
	return m_buttons.emplace(name, std::move(button)).first->second.get();
}

void InputScheme::RemoveButton(const std::string &name) {
	auto it = m_buttons.find(name);
	if (it != m_buttons.end())
		m_buttons.erase(it);
}

const Node &operator>>(const Node &node, InputScheme &inputScheme) {
	node["axes"].Get(inputScheme.m_axes);
	node["buttons"].Get(inputScheme.m_buttons);
	return node;
}

Node &operator<<(Node &node, const InputScheme &inputScheme) {
	node["axes"].Set(inputScheme.m_axes);
	node["buttons"].Set(inputScheme.m_buttons);
	return node;
}

void InputScheme::MoveDelegateOwnership(InputScheme *other) {
	if (!other) return;
	for (auto &[axisName, axis] : other->m_axes)
		GetAxis(axisName)->OnAxis().MoveFunctions(axis->OnAxis());
	for (auto &[buttonName, button] : other->m_buttons)
		GetButton(buttonName)->OnButton().MoveFunctions(button->OnButton());
}
}
