#pragma once

#include <map>
#include "Engine/Engine.hpp"

typedef struct ALCdevice_struct ALCdevice;

typedef struct ALCcontext_struct ALCcontext;

namespace acid
{
	enum SoundType
	{
		SOUND_TYPE_GENERAL = 0,
		SOUND_TYPE_EFFECT = 1,
		SOUND_TYPE_MUSIC = 2
	};

	/// <summary>
	/// A module used for loading, managing and playing a variety of different sound types.
	/// </summary>
	class ACID_EXPORT Audio :
		public IModule
	{
	private:
		ALCdevice *m_alDevice;
		ALCcontext *m_alContext;

		float m_masterGain;
		std::map<SoundType, float> m_gains;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Audio *Get() { return Engine::Get()->GetModule<Audio>(); }

		Audio();

		~Audio();

		void Update() override;

		ACID_HIDDEN static std::string StringifyResultAl(const int32_t &result);

		ACID_HIDDEN static void CheckAl(const int32_t &result);

		ACID_HIDDEN ALCdevice *GetDevice() const { return m_alDevice; }

		ACID_HIDDEN ALCcontext *GetContext() const { return m_alContext; }

		float GetMasterGain() const { return m_masterGain; }

		void SetMasterGain(const float &masterGain) { m_masterGain = masterGain; }

		float GetTypeGain(const SoundType &type) const;

		void SetTypeGain(const SoundType &type, const float &volume);
	};
}
