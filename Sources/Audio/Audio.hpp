#pragma once

#ifdef ACID_BUILD_MACOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include "Engine/Engine.hpp"

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
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Audio *Get()
		{
			return Engine::Get()->GetModule<Audio>();
		}

		/// <summary>
		/// Creates a new audio module.
		/// </summary>
		Audio();

		/// <summary>
		/// Deconstructor for the audio module.
		/// </summary>
		~Audio();

		void Update() override;

		static std::string StringifyResultAl(const int &result);

		static void CheckAl(const int &result);

		ALCdevice *GetDevice() const { return m_alDevice; }

		ALCcontext *GetContext() const { return m_alContext; }
	};
}
