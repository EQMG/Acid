#include "InputScheme.hpp"

#include "Files/Json/Json.hpp"
#include "AxisButton.hpp"
#include "AxisCompound.hpp"
#include "AxisJoystick.hpp"
#include "AxisMouse.hpp"
#include "ButtonAxis.hpp"
#include "ButtonCompound.hpp"
#include "ButtonJoystick.hpp"
#include "ButtonKeyboard.hpp"
#include "ButtonMouse.hpp"

namespace acid {
InputScheme::InputScheme(const std::filesystem::path &filename) :
	m_file(filename) {
	m_file.Load();
	*m_file.GetNode() >> *this;

	File argsFile("ArgumentDescriptionMap.json");
	*argsFile.GetNode() << ArgumentDescriptionMap;
	argsFile.Write(Node::Format::Beautified);

	File testOutFile(filename);
	*testOutFile.GetNode() << *this;
	testOutFile.Write(Node::Format::Beautified);
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
	node["joysticks"].Get(inputScheme.m_joysticks);
	// Need to use our factories to fill axes and buttons maps.
	for (const auto &axis : node["axes"]->GetProperties()) {
		inputScheme.m_axes.emplace(axis["name"].Get<std::string>(), inputScheme.ParseAxis(axis));
	}
	for (const auto &button : node["buttons"]->GetProperties()) {
		inputScheme.m_buttons.emplace(button["name"].Get<std::string>(), inputScheme.ParseButton(button));

	}
	return node;
}

Node &operator<<(Node &node, const InputScheme &inputScheme) {
	node["joysticks"].Set(inputScheme.m_joysticks);
	for (const auto &[axisName, axis] : inputScheme.m_axes) {
		auto axisNode = node["axes"]->AddProperty();
		axisNode["name"].Set(axisName);
		inputScheme.WriteAxis(axis.get(), axisNode);
	}
	for (const auto &[buttonName, button] : inputScheme.m_buttons) {
		auto buttonNode = node["buttons"]->AddProperty();
		buttonNode["name"].Set(buttonName);
		inputScheme.WriteButton(button.get(), buttonNode);
	}
	return node;
}

std::unique_ptr<Axis> InputScheme::ParseAxis(const Node &node) const {
	auto type = node["type"].Get<std::string>();
	std::unique_ptr<Axis> result;
	if (type == "axisButton") {
		result = std::make_unique<AxisButton>(ParseButton(node["negative"]), ParseButton(node["positive"]));
	} else if (type == "axisCompound") {
		std::vector<std::unique_ptr<Axis>> axes;
		for (const auto &axis : node["axes"]->GetProperties()) {
			axes.emplace_back(ParseAxis(axis));
		}
		result = std::make_unique<AxisCompound>(std::move(axes));
	} else if (type == "axisJoystick") {
		auto port = *GetJoystickPort(node["port"].Get<std::string>());
		result = std::make_unique<AxisJoystick>(port, node["axis"].Get<JoystickAxis>());
	} else if (type == "axisMouse") {
		result = std::make_unique<AxisMouse>(node["axis"].Get<uint8_t>());
	}

	if (result)
		result->SetScale(node["scale"].Get<float>(1.0f));
	return result;
}

std::unique_ptr<Button> InputScheme::ParseButton(const Node &node) const {
	auto type = node["type"].Get<std::string>();
	std::unique_ptr<Button> result;

	if (type == "buttonAxis") {
		result = std::make_unique<ButtonAxis>(ParseAxis(node["axis"]), node["min"].Get<float>(), node["max"].Get<float>());
	} else if (type == "buttonCompound") {
		std::vector<std::unique_ptr<Button>> buttons;
		for (const auto &button : node["buttons"]->GetProperties()) {
			buttons.emplace_back(ParseButton(button));
		}
		result = std::make_unique<ButtonCompound>(std::move(buttons), node["useAnd"].Get<bool>(false));
	} else if (type == "buttonJoystick") {
		auto port = *GetJoystickPort(node["port"].Get<std::string>());
		result = std::make_unique<ButtonJoystick>(port, node["button"].Get<JoystickButton>());
	} else if (type == "buttonKeyboard") {
		result = std::make_unique<ButtonKeyboard>(node["key"].Get<Key>());
	} else if (type == "buttonMouse") {
		result = std::make_unique<ButtonMouse>(node["key"].Get<MouseButton>());
	}

	if (result)
		result->SetInverted(node["inverted"].Get<bool>(false));
	return result;
}

void InputScheme::WriteAxis(const Axis *axis, Node &node) const {
}

void InputScheme::WriteButton(const Button *axis, Node &node) const {
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
