#pragma once

#include "Engine/Engine.hpp"

typedef struct ALCdevice_struct ALCdevice;

typedef struct ALCcontext_struct ALCcontext;

namespace acid
{
	/// <summary>
	/// A module used for loading, managing and playing a variety of different sound types.
	/// </summary>
	class ACID_EXPORT Audio :
		public IModule
	{
	private:
		ALCdevice *m_alDevice;
		ALCcontext *m_alContext;

		float m_volume;
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

		float GetVolume() const { return m_volume; }

		void SetVolume(const float &volume) { m_volume = volume; }
	};
}
