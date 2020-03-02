#pragma once

#include "Engine/Engine.hpp"
#include "Utils/Delegate.hpp"

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

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

	ACID_NO_EXPORT ALCdevice *GetDevice() const { return device; }
	ACID_NO_EXPORT ALCcontext *GetContext() const { return context; }

	float GetGain(Type type) const;
	void SetGain(Type type, float volume);

	/**
	 * Called when a gain value has been modified.
	 * @return The delegate.
	 */
	Delegate<void(Type, float)> &OnGain() { return onGain; }

private:
	ALCdevice *device = nullptr;
	ALCcontext *context = nullptr;

	std::map<Type, float> gains;

	Delegate<void(Type, float)> onGain;
};
}
