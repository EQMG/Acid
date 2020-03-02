#pragma once

#include "Devices/Joysticks.hpp"
#include "Files/File.hpp"
#include "InputAxis.hpp"
#include "InputButton.hpp"

namespace acid {
/**
 * Class is used to abstract and wrap input methods inside a serializable factory.
 */
class ACID_EXPORT InputScheme {
	friend class Input;
public:
	using AxisMap = std::map<std::string, std::unique_ptr<InputAxis>>;
	using ButtonMap = std::map<std::string, std::unique_ptr<InputButton>>;
	using JoystickMap = std::map<std::string, JoystickPort>;

	InputScheme() = default;
	explicit InputScheme(const std::filesystem::path &filename);

	InputAxis *GetAxis(const std::string &name);
	InputAxis *AddAxis(const std::string &name, std::unique_ptr<InputAxis> &&axis);
	void RemoveAxis(const std::string &name);

	InputButton *GetButton(const std::string &name);
	InputButton *AddButton(const std::string &name, std::unique_ptr<InputButton> &&button);
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
