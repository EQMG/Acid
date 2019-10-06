#pragma once

#include "Devices/Joysticks.hpp"
#include "Files/File.hpp"
#include "Axis.hpp"
#include "Button.hpp"

namespace acid {
class ACID_EXPORT InputScheme {
public:
	struct Argument {
		std::string name;
		std::string type;
		std::string description;

		friend const Node &operator>>(const Node &node, Argument &argument) {
			node["name"].Get(argument.name);
			node["type"].Get(argument.type);
			node["description"].Get(argument.description);
			return node;
		}
		friend Node &operator<<(Node &node, const Argument &argument) {
			node["name"].Set(argument.name);
			node["type"].Set(argument.type);
			node["description"].Set(argument.description);
			return node;
		}

	};
	using ArgumentDescription = std::vector<Argument>;
	
	explicit InputScheme(const std::filesystem::path &filename);

	Axis *GetAxis(const std::string &name) const;
	Button *GetButton(const std::string &name) const;
	std::optional<JoystickPort> GetJoystickPort(const std::string &name) const;

	static ArgumentDescription GetArgumentDescription(const std::string &name);
	
	friend const Node &operator>>(const Node &node, InputScheme &inputScheme);
	friend Node &operator<<(Node &node, const InputScheme &inputScheme);

private:
	static const std::map<std::string, ArgumentDescription> ArgumentDescriptionMap;
	
	File m_file;

	std::map<std::string, std::unique_ptr<Axis>> m_axes;
	std::map<std::string, std::unique_ptr<Button>> m_buttons;
	std::map<std::string, JoystickPort> m_joysticks;
};
}
