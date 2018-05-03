#pragma once

#ifdef FL_BUILD_MACOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <STB/stb_vorbis.h>
#include "Engine/Engine.hpp"

namespace fl
{
	/// <summary>
	/// A module used for loading, managing and playing a variety of different sound types.
	/// </summary>
	class FL_EXPORT Audio :
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
			return reinterpret_cast<Audio *>(Engine::Get()->GetModule("audio"));
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

		static void ErrorAl(const ALenum &result);

		static ALuint LoadFileWav(const std::string &filename);

		static ALuint LoadFileOgg(const std::string &filename);
	};
}
