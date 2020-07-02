#include "Inputs.hpp"

#include <iomanip>

namespace acid {
Inputs::Inputs() :
	nullScheme(std::make_unique<InputScheme>()),
	currentScheme(nullScheme.get()) {
}

void Inputs::Update() {
}

InputScheme *Inputs::GetScheme(const std::string &name) const {
	auto it = schemes.find(name);
	if (it == schemes.end()) {
		Log::Error("Could not find input scheme: ", std::quoted(name), '\n');
		return nullptr;
	}
	return it->second.get();
}

InputScheme *Inputs::AddScheme(const std::string &name, std::unique_ptr<InputScheme> &&scheme, bool setCurrent) {
	auto inputScheme = schemes.emplace(name, std::move(scheme)).first->second.get();
	if (!currentScheme || setCurrent)
		SetScheme(inputScheme);
	return inputScheme;
} 

void Inputs::RemoveScheme(const std::string &name) {
	auto it = schemes.find(name);
	if (currentScheme == it->second.get())
		SetScheme(nullptr);
	if (it != schemes.end())
		schemes.erase(it);
	// If we have no current scheme grab some random one from the map.
	if (!currentScheme && !schemes.empty())
		currentScheme = schemes.begin()->second.get();
}

void Inputs::SetScheme(InputScheme *scheme) {
	if (!scheme) scheme = nullScheme.get();
	// We want to preserve delegate function pointers from the current scheme to the new one.
	scheme->MoveDelegateOwnership(currentScheme);
	currentScheme = scheme;
}

void Inputs::SetScheme(const std::string &name) {
	auto scheme = GetScheme(name);
	if (!scheme) return;
	SetScheme(scheme);
}

InputAxis *Inputs::GetAxis(const std::string &name) const {
	if (currentScheme)
		return currentScheme->GetAxis(name);
	return nullptr;
}

InputButton *Inputs::GetButton(const std::string &name) const {
	if (currentScheme)
		return currentScheme->GetButton(name);
	return nullptr;
}
}
