#pragma once

#include "Engine/Engine.hpp"
#include "InputScheme.hpp"

namespace acid {
/**
 * @brief Module used for managing abstract inputs organized in schemes.
 */
class ACID_EXPORT Input : public Module::Registrar<Input> {
public:
	Input();

	void Update() override;

	InputScheme *GetScheme(const std::string &name) const;
	InputScheme *GetScheme() const;
	InputScheme *AddScheme(const std::string &name, std::unique_ptr<InputScheme> &&scheme, bool setCurrent = false);
	void RemoveScheme(const std::string &name);
	void SetScheme(InputScheme *scheme);
	void SetScheme(const std::string &name);

	Axis *GetAxis(const std::string &name) const;
	Button *GetButton(const std::string &name) const;

private:
	std::map<std::string, std::unique_ptr<InputScheme>> m_schemes;
	std::unique_ptr<InputScheme> m_nullScheme;
	InputScheme *m_currentScheme = nullptr;
};
}
