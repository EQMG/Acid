#pragma once

#include "Engine/Engine.hpp"
#include "InputScheme.hpp"

namespace acid {
/**
 * @brief Module used for managing abstract inputs organized in schemes.
 */
class ACID_INPUT_EXPORT Inputs : public Module::Registrar<Inputs> {
	inline static const bool Registered = Register(Stage::Pre, Requires<Windows, Joysticks>());
public:
	Inputs();

	void Update() override;

	InputScheme *GetScheme() const { return currentScheme; }
	InputScheme *GetScheme(const std::string &name) const;
	InputScheme *AddScheme(const std::string &name, std::unique_ptr<InputScheme> &&scheme, bool setCurrent = false);
	void RemoveScheme(const std::string &name);
	void SetScheme(InputScheme *scheme);
	void SetScheme(const std::string &name);

	InputAxis *GetAxis(const std::string &name) const;
	InputButton *GetButton(const std::string &name) const;

private:
	std::map<std::string, std::unique_ptr<InputScheme>> schemes;
	std::unique_ptr<InputScheme> nullScheme;
	InputScheme *currentScheme = nullptr;
};
}
