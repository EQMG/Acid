#pragma once

#include "Engine/Engine.hpp"
#include "Utils/Delegate.hpp"

namespace acid {
/**
 * @brief Module used for loading, managing and playing a variety of different sound types.
 */
class ACID_EXPORT Audio : public Module::Registrar<Audio, Module::Stage::Pre> {
public:
	enum class Type {
		Master, General, Effect, Music
	};

	Audio();
	~Audio();

	void Update() override;

	ACID_NO_EXPORT static std::string StringifyResultAl(int32_t result);
	ACID_NO_EXPORT static void CheckAl(int32_t result);

	float GetGain(Type type) const;
	void SetGain(Type type, float volume);

	/**
	 * Called when a gain value has been modified.
	 * @return The delegate.
	 */
	Delegate<void(Type, float)> &OnGain() { return onGain; }

private:
	// TODO: Only using p-impl because of signature differences from OpenAL and OpenALSoft.
	struct _intern;
	std::unique_ptr<_intern> impl;

	std::map<Type, float> gains;

	Delegate<void(Type, float)> onGain;
};
}
