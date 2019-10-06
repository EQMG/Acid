#pragma once

#include "Devices/Joysticks.hpp"
#include "Files/File.hpp"
#include "Axis.hpp"
#include "Button.hpp"

namespace acid {
class ACID_EXPORT InputScheme {
public:
	explicit InputScheme(File &&file);
	
	Axis *GetAxis(const std::string &name) const;
	Button *GetButton(const std::string &name) const;
	std::optional<JoystickPort> GetJoystickPort(const std::string &name) const;

	friend const Node &operator>>(const Node &node, InputScheme &inputScheme);
	friend Node &operator<<(Node &node, const InputScheme &inputScheme);

private:
	File m_file;

	std::map<std::string, std::unique_ptr<Axis>> m_axes;
	std::map<std::string, std::unique_ptr<Button>> m_buttons;
	std::map<std::string, JoystickPort> m_joysticks;
};
}
