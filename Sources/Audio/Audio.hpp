#pragma once

#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

namespace acid {
/**
 * @brief Module used for loading, managing and playing a variety of different sound types.
 */
class ACID_EXPORT
Audio
:
public
Module
{
public:
	enum class Type
	{
		Master, General, Effect, Music
	};

	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Audio *Get() { return Engine::Get()->GetModule<Audio>(); }

	Audio();

	~Audio();

	void Update() override;

	ACID_NO_EXPORT static std::string StringifyResultAl(int32_t result);

	ACID_NO_EXPORT static void CheckAl(int32_t result);
	
	ACID_NO_EXPORT ALCdevice *GetDevice() const { return m_device; }

	ACID_NO_EXPORT ALCcontext *GetContext() const { return m_context; }

	float GetGain(const Type &type) const;

	void SetGain(const Type &type, float volume);

	/**
	 * Called when a gain value has been modified.
	 * @return The delegate.
	 */
	Delegate<void(Type, float)> &OnGain() { return m_onGain; }

private:
	ALCdevice *m_device = nullptr;
	ALCcontext *m_context = nullptr;

	std::map<Type, float> m_gains;

	Delegate<void(Type, float)> m_onGain;
};
}
