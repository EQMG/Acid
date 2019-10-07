#include "Input.hpp"

namespace acid {
Input::Input() {
}

void Input::Update() {
}

InputScheme *Input::GetScheme(const std::string &name) const {
	auto it = m_schemes.find(name);
	if (it == m_schemes.end())
		return nullptr;
	return it->second.get();
}

InputScheme *Input::AddScheme(const std::string &name, std::unique_ptr<InputScheme> &&scheme) {
	auto inputScheme = m_schemes.emplace(name, std::move(scheme)).first->second.get();
	if (m_currentScheme == nullptr)
		m_currentScheme = inputScheme;
	return inputScheme;
} 

void Input::RemoveScheme(const std::string &name) {
	auto it = m_schemes.find(name);
	if (m_currentScheme == it->second.get())
		m_currentScheme = nullptr;
	if (it != m_schemes.end())
		m_schemes.erase(it);
	// If we have no current scheme grab some random one from the map.
	if (!m_currentScheme && !m_schemes.empty())
		m_currentScheme = m_schemes.begin()->second.get();
}

void Input::SetCurrentScheme(const std::string &name) {
	m_currentScheme = GetScheme(name);
}

Axis *Input::GetAxis(const std::string &name) const {
	if (m_currentScheme)
		return m_currentScheme->GetAxis(name);
	return nullptr;
}

Button *Input::GetButton(const std::string &name) const {
	if (m_currentScheme)
		return m_currentScheme->GetButton(name);
	return nullptr;
}
}
