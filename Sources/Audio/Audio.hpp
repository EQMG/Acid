#pragma once

#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

namespace acid
{
/**
 * M@brief odule used for loading, managing and playing a variety of different sound types.
 */
class ACID_EXPORT Audio :
	public Module
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
	static Audio *Get() { return Engine::Get()->GetModuleManager().Get<Audio>(); }

	Audio();

	~Audio();

	void Update() override;

	ACID_HIDDEN static std::string StringifyResultAl(const int32_t &result);

	ACID_HIDDEN static void CheckAl(const int32_t &result);

	ACID_HIDDEN ALCdevice *GetDevice() const { return m_alDevice; }

	ACID_HIDDEN ALCcontext *GetContext() const { return m_alContext; }

	float GetGain(const Type &type) const;

	void SetGain(const Type &type, const float &volume);

	/**
	 * Called when a gain value has been modified.
	 * @return The delegate.
	 */
	Delegate<void(Type, float)> &OnGain() { return m_onGain; }

private:
	ALCdevice *m_alDevice;
	ALCcontext *m_alContext;

	std::map<Type, float> m_gains;

	Delegate<void(Type, float)> m_onGain;
};
}
