#include "InputScheme.hpp"

namespace acid {
InputScheme::InputScheme(File &&file) :
	m_file(std::move(file)) {
	*m_file.GetNode() >> *this;
}

Axis *InputScheme::GetAxis(const std::string &name) const {
	auto it = m_axes.find(name);
	if (it == m_axes.end())
		return nullptr;
	return it->second.get();
}

Button *InputScheme::GetButton(const std::string &name) const {
	auto it = m_buttons.find(name);
	if (it == m_buttons.end())
		return nullptr;
	return it->second.get();
}

std::optional<JoystickPort> InputScheme::GetJoystickPort(const std::string &name) const {
	auto it = m_joysticks.find(name);
	if (it == m_joysticks.end())
		return {};
	return it->second;
}

const Node &operator>>(const Node &node, InputScheme &inputScheme) {
	// Need to use our factories to fill axes and buttons maps.
	//node["axes"].Get(inputScheme.m_axes);
	//node["buttons"].Get(inputScheme.m_buttons);
	node["joysticks"].Get(inputScheme.m_joysticks);
	return node;
}

Node &operator<<(Node &node, const InputScheme &inputScheme) {
	node["axes"].Set(inputScheme.m_axes);
	node["buttons"].Set(inputScheme.m_buttons);
	node["joysticks"].Set(inputScheme.m_joysticks);
	return node;
}
}
