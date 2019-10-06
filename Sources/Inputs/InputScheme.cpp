#include "InputScheme.hpp"

#include "Files/Json/Json.hpp"

namespace acid {
InputScheme::InputScheme(const std::filesystem::path &filename) :
	m_file(filename) {
	m_file.Load();
	*m_file.GetNode() >> *this;

	File argsFile("ArgumentDescriptionMap.json");
	*argsFile.GetNode() << ArgumentDescriptionMap;
	argsFile.Write(Node::Format::Beautified);
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

InputScheme::ArgumentDescription InputScheme::GetArgumentDescription(const std::string &name) {
	auto it = ArgumentDescriptionMap.find(name);
	if (it == ArgumentDescriptionMap.end())
		return {};
	return it->second;
}

const Node &operator>>(const Node &node, InputScheme &inputScheme) {
	// Need to use our factories to fill axes and buttons maps.
	for (auto axis : node["axes"]->GetProperties()) {

	}
	for (auto button : node["buttons"]->GetProperties()) {

	}
	node["joysticks"].Get(inputScheme.m_joysticks);
	return node;
}

Node &operator<<(Node &node, const InputScheme &inputScheme) {
	node["axes"].Set(inputScheme.m_axes);
	node["buttons"].Set(inputScheme.m_buttons);
	node["joysticks"].Set(inputScheme.m_joysticks);
	return node;
}

const std::map<std::string, InputScheme::ArgumentDescription> InputScheme::ArgumentDescriptionMap = {
	{
		"axisButton", {
			{"negative", "button", "Button when pressed makes the amount read negative"},
			{"positive", "button", "Button when pressed makes the amount read positive"},
			{"scale", "float", "Output amount scalar"}
		}
	},
	{
		"axisCompound", {
			{"axes", "axis[]", "The axes that will be combined into a compound axis"},
			{"scale", "float", "Output amount scalar"}
		}
	},
	{
		"axisJoystick", {
			{"port", "int", "The joystick port name"},
			{"axis", "int", "The axis on the joystick being checked"},
			{"scale", "float", "Output amount scalar"}
		}
	},
	{
		"axisMouse", {
			{"axis", "axis", "The axis on the mouse delta is being checked"},
			{"scale", "float", "Output amount scalar"}
		}
	},
	{
		"buttonAxis", {
			{"axis", "axis", "The axis to sample"},
			{"min", "float", "Lower axis value bound"},
			{"max", "float", "Upper axis value bound"},
			{"inverted", "bool", "If the down reading will be inverted"}
		}
	},
	{
		"buttonCompound", {
			{"buttons", "button[]", "The buttons that will be combined into a compound button"},
			{"useAnd", "bool", "If must be down for a down reading, or just one"},
			{"inverted", "bool", "If the down reading will be inverted"}
		}
	},
	{
		"buttonJoystick", {
			{"port", "int", "The joystick port name"},
			{"axis", "bool", "The button on the joystick being checked"},
			{"inverted", "bool", "If the down reading will be inverted"}
		}
	},
	{
		"buttonKeyboard", {
			{"key", "int", "The key on the keyboard being checked"},
			{"inverted", "bool", "If the down reading will be inverted"}
		}
	},
	{
		"buttonMouse", {
			{"key", "int", "The mouse button on the mouse being checked"},
			{"inverted", "bool", "If the down reading will be inverted"}
		}
	},
	{
		"hatJoystick", {
			{"port", "int", "The joystick port name"},
			{"axis", "int", "The hat on the joystick being checked"},
			{"hatFlags", "int", "The bits used to check if the hat is a down reading"},
			{"scale", "float", "Output amount scalar"},
			{"inverted", "bool", "If the down reading will be inverted"}
		}
	}
};
}
