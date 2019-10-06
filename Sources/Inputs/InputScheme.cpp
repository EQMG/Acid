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
#include "HatJoystick.hpp"

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

Axis *InputScheme::AddAxis(const std::string &name, std::unique_ptr<Axis> &&axis) {
	return m_axes.emplace(name, std::move(axis)).first->second.get();
}

void InputScheme::RemoveAxis(const std::string &name) {
	auto it = m_axes.find(name);
	if (it != m_axes.end())
		m_axes.erase(it);
}

Button *InputScheme::GetButton(const std::string &name) const {
	auto it = m_buttons.find(name);
	if (it == m_buttons.end())
		return nullptr;
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

std::optional<JoystickPort> InputScheme::GetJoystickPort(const std::string &name) const {
	auto it = m_joysticks.find(name);
	if (it == m_joysticks.end())
		return {};
	return it->second;
}

std::string InputScheme::GetJoystickPortName(JoystickPort port) const {
	for (const auto &joystick : m_joysticks)
		if (joystick.second == port)
			return joystick.first;
	return String::To(port);
}

void InputScheme::AddJoystickPort(const std::string &name, JoystickPort port) {
	m_joysticks.emplace(name, port);
}

void InputScheme::RemoveJoystickPort(const std::string &name) {
	auto it = m_joysticks.find(name);
	if (it != m_joysticks.end())
		m_joysticks.erase(it);
}

InputScheme::ArgumentDescription InputScheme::GetArgumentDescription(const std::string &name) {
	auto it = ArgumentDescriptionMap.find(name);
	if (it == ArgumentDescriptionMap.end())
		return {};
	return it->second;
}

const Node &operator>>(const Node &node, InputScheme::Argument &argument) {
	node["name"].Get(argument.name);
	node["type"].Get(argument.type);
	node["description"].Get(argument.description);
	return node;
}

Node &operator<<(Node &node, const InputScheme::Argument &argument) {
	node["name"].Set(argument.name);
	node["type"].Set(argument.type);
	node["description"].Set(argument.description);
	return node;
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
		auto &axisNode = node["axes"]->AddProperty();
		axisNode["name"] = axisName;
		inputScheme.WriteAxis(axis.get(), axisNode);
	}
	for (const auto &[buttonName, button] : inputScheme.m_buttons) {
		auto &buttonNode = node["buttons"]->AddProperty();
		buttonNode["name"] = buttonName;
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
	} else if (type == "hatJoystick") {
		auto port = *GetJoystickPort(node["port"].Get<std::string>());
		result = std::make_unique<HatJoystick>(port, node["hat"].Get<JoystickHat>(), node["hatFlags"].Get<uint8_t>());
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
	} else if (type == "hatJoystick") {
		auto port = *GetJoystickPort(node["port"].Get<std::string>());
		result = std::make_unique<HatJoystick>(port, node["hat"].Get<JoystickHat>(), node["hatFlags"].Get<uint8_t>());
	}

	if (result)
		result->SetInverted(node["inverted"].Get<bool>(false));
	return result;
}

void InputScheme::WriteAxis(const Axis *axis, Node &node) const {
	if (axis == nullptr) return;
	if (auto axisButton = dynamic_cast<const AxisButton *>(axis)) {
		node["type"] = "axisButton";
		WriteButton(axisButton->GetPositive(), node["positive"]);
		WriteButton(axisButton->GetNegative(), node["negative"]);
	} else if (auto axisCompound = dynamic_cast<const AxisCompound *>(axis)) {
		node["type"] = "axisCompound";
		for (const auto &childAxis : axisCompound->GetAxes())
			WriteAxis(childAxis.get(), node["axes"]->AddProperty());
	} else if (auto axisJoystick = dynamic_cast<const AxisJoystick *>(axis)) {
		node["type"] = "axisJoystick";
		node["port"] = GetJoystickPortName(axisJoystick->GetPort());
		node["axis"] = axisJoystick->GetAxis();
	} else if (auto axisMouse = dynamic_cast<const AxisMouse *>(axis)) {
		node["type"] = "axisMouse";
		node["axis"] = axisMouse->GetAxis();
	} else if (auto hatJoystick = dynamic_cast<const HatJoystick *>(axis)) {
		node["type"] = "hatJoystick";
		node["port"] = GetJoystickPortName(hatJoystick->GetPort());
		node["hat"] = hatJoystick->GetHat();
		node["hatFlags"] = hatJoystick->GetHatFlags();
	}
	
	node["scale"] = axis->GetScale();
}

void InputScheme::WriteButton(const Button *button, Node &node) const {
	if (button == nullptr) return;
	if (auto buttonAxis = dynamic_cast<const ButtonAxis *>(button)) {
		node["type"] = "buttonAxis";
		WriteAxis(buttonAxis->GetAxis(), node["axis"]);
		node["min"] = buttonAxis->GetMin();
		node["max"] = buttonAxis->GetMax();
	} else if (auto buttonCompound = dynamic_cast<const ButtonCompound *>(button)) {
		node["type"] = "buttonCompound";
		for (const auto &childButton : buttonCompound->GetButtons())
			WriteButton(childButton.get(), node["buttons"]->AddProperty());
	} else if (auto buttonJoystick = dynamic_cast<const ButtonJoystick *>(button)) {
		node["type"] = "buttonJoystick";
		node["port"] = GetJoystickPortName(buttonJoystick->GetPort());
		node["button"] = buttonJoystick->GetButton();
	} else if (auto buttonKeyboard = dynamic_cast<const ButtonKeyboard *>(button)) {
		node["type"] = "buttonKeyboard";
		node["key"] = buttonKeyboard->GetKey();
	} else if (auto buttonMouse = dynamic_cast<const ButtonMouse *>(button)) {
		node["type"] = "buttonMouse";
		node["button"] = buttonMouse->GetButton();
	} else if (auto hatJoystick = dynamic_cast<const HatJoystick *>(button)) {
		node["type"] = "hatJoystick";
		node["port"] = GetJoystickPortName(hatJoystick->GetPort());
		node["hat"] = hatJoystick->GetHat();
		node["hatFlags"] = hatJoystick->GetHatFlags();
	}

	node["inverted"] = button->IsInverted();
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
