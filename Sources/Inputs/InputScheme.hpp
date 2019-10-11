#pragma once

#include "Devices/Joysticks.hpp"
#include "Files/File.hpp"
#include "Axes/Axis.hpp"
#include "Buttons/Button.hpp"

namespace acid {
/**
 * Class is used to abstract and wrap input methods inside a serializable factory.
 */
class ACID_EXPORT InputScheme {
	friend class Input;
public:
	struct Argument {
		std::string name;
		std::string type;
		std::string description;

		friend const Node &operator>>(const Node &node, Argument &argument);
		friend Node &operator<<(Node &node, const Argument &argument);
	};
	using ArgumentDescription = std::vector<Argument>;

	using AxisMap = std::map<std::string, std::unique_ptr<Axis>>;
	using ButtonMap = std::map<std::string, std::unique_ptr<Button>>;
	using JoystickMap = std::map<std::string, JoystickPort>;

	InputScheme() = default;
	explicit InputScheme(const std::filesystem::path &filename);

	std::optional<JoystickPort> GetJoystickPort(const std::string &name) const;
	std::string GetJoystickPortName(JoystickPort port) const;
	void AddJoystickPort(const std::string &name, JoystickPort port);
	void RemoveJoystickPort(const std::string &name);

	Axis *GetAxis(const std::string &name);
	Axis *AddAxis(const std::string &name, std::unique_ptr<Axis> &&axis);
	void RemoveAxis(const std::string &name);

	Button *GetButton(const std::string &name);
	Button *AddButton(const std::string &name, std::unique_ptr<Button> &&button);
	void RemoveButton(const std::string &name);

	const File &GetFile() const { return m_file; }
	
	static ArgumentDescription GetArgumentDescription(const std::string &name);
	
	friend const Node &operator>>(const Node &node, InputScheme &inputScheme);
	friend Node &operator<<(Node &node, const InputScheme &inputScheme);

private:
	static const std::map<std::string, ArgumentDescription> ArgumentDescriptionMap;

	void MoveDelegateOwnership(InputScheme *other);

	std::unique_ptr<Axis> ParseAxis(const Node &node) const;
	std::unique_ptr<Button> ParseButton(const Node &node) const;

	void WriteAxis(const Axis *axis, Node &node) const;
	void WriteButton(const Button *button, Node &node) const;

	JoystickMap m_joysticks;
	AxisMap m_axes;
	ButtonMap m_buttons;

	File m_file;
};
}
