#include "Input.hpp"

#include <iomanip>

namespace acid {
Input::Input() :
	nullScheme(std::make_unique<InputScheme>()),
	currentScheme(nullScheme.get()) {
}

void Input::Update() {
}

InputScheme *Input::GetScheme(const std::string &name) const {
	auto it = schemes.find(name);
	if (it == schemes.end()) {
		Log::Error("Could not find input scheme: ", std::quoted(name), '\n');
		return nullptr;
	}
	return it->second.get();
}

InputScheme *Input::AddScheme(const std::string &name, std::unique_ptr<InputScheme> &&scheme, bool setCurrent) {
	auto inputScheme = schemes.emplace(name, std::move(scheme)).first->second.get();
	if (!currentScheme || setCurrent)
		SetScheme(inputScheme);
	return inputScheme;
} 

void Input::RemoveScheme(const std::string &name) {
	auto it = schemes.find(name);
	if (currentScheme == it->second.get())
		SetScheme(nullptr);
	if (it != schemes.end())
		schemes.erase(it);
	// If we have no current scheme grab some random one from the map.
	if (!currentScheme && !schemes.empty())
		currentScheme = schemes.begin()->second.get();
}

void Input::SetScheme(InputScheme *scheme) {
	if (!scheme) scheme = nullScheme.get();
	// We want to preserve delegate function pointers from the current scheme to the new one.
	scheme->MoveDelegateOwnership(currentScheme);
	currentScheme = scheme;
}

void Input::SetScheme(const std::string &name) {
	auto scheme = GetScheme(name);
	if (!scheme) return;
	SetScheme(scheme);
}

InputAxis *Input::GetAxis(const std::string &name) const {
	if (currentScheme)
		return currentScheme->GetAxis(name);
	return nullptr;
}

InputButton *Input::GetButton(const std::string &name) const {
	if (currentScheme)
		return currentScheme->GetButton(name);
	return nullptr;
}
}
