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
	InputScheme *GetScheme() const { return m_currentScheme; }
	InputScheme *AddScheme(const std::string &name, std::unique_ptr<InputScheme> &&scheme);
	void RemoveScheme(const std::string &name);
	void SetCurrentScheme(const std::string &name);

	Axis *GetAxis(const std::string &name) const;
	Button *GetButton(const std::string &name) const;
	
private:
	std::map<std::string, std::unique_ptr<InputScheme>> m_schemes;

	InputScheme *m_currentScheme = nullptr;
};
}
