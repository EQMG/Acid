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
	using AxisMap = std::map<std::string, std::unique_ptr<Axis>>;
	using ButtonMap = std::map<std::string, std::unique_ptr<Button>>;
	using JoystickMap = std::map<std::string, JoystickPort>;

	InputScheme() = default;
	explicit InputScheme(const std::filesystem::path &filename);

	Axis *GetAxis(const std::string &name);
	Axis *AddAxis(const std::string &name, std::unique_ptr<Axis> &&axis);
	void RemoveAxis(const std::string &name);

	Button *GetButton(const std::string &name);
	Button *AddButton(const std::string &name, std::unique_ptr<Button> &&button);
	void RemoveButton(const std::string &name);

	const File &GetFile() const { return file; }
	
	friend const Node &operator>>(const Node &node, InputScheme &inputScheme);
	friend Node &operator<<(Node &node, const InputScheme &inputScheme);

private:
	void MoveDelegateOwnership(InputScheme *other);

	AxisMap axes;
	ButtonMap buttons;

	File file;
};
}
