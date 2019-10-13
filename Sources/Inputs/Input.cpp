#include "Input.hpp"

namespace acid {
Input::Input() :
	m_nullScheme(std::make_unique<InputScheme>()),
	m_currentScheme(m_nullScheme.get()) {
}

void Input::Update() {
}

InputScheme *Input::GetScheme(const std::string &name) const {
	auto it = m_schemes.find(name);
	if (it == m_schemes.end()) {
		Log::Error("Could not find input scheme: ", std::quoted(name), '\n');
		return nullptr;
	}
	return it->second.get();
}

InputScheme *Input::GetScheme() const {
	return m_currentScheme;
}

InputScheme *Input::AddScheme(const std::string &name, std::unique_ptr<InputScheme> &&scheme, bool setCurrent) {
	auto inputScheme = m_schemes.emplace(name, std::move(scheme)).first->second.get();
	if (m_currentScheme == nullptr || setCurrent)
		SetScheme(inputScheme);
	return inputScheme;
} 

void Input::RemoveScheme(const std::string &name) {
	auto it = m_schemes.find(name);
	if (m_currentScheme == it->second.get())
		SetScheme(nullptr);
	if (it != m_schemes.end())
		m_schemes.erase(it);
	// If we have no current scheme grab some random one from the map.
	if (!m_currentScheme && !m_schemes.empty())
		m_currentScheme = m_schemes.begin()->second.get();
}

void Input::SetScheme(InputScheme *scheme) {
	if (!scheme) scheme = m_nullScheme.get();
	// We want to preserve delegate function pointers from the current scheme to the new one.
	scheme->MoveDelegateOwnership(m_currentScheme);
	m_currentScheme = scheme;
}

void Input::SetScheme(const std::string &name) {
	auto scheme = GetScheme(name);
	if (!scheme) return;
	SetScheme(scheme);
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
