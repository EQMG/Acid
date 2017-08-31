#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <map>

#include <al/al.h>
#include <al/alc.h>

#include "../camera/camera.hpp"
#include "../engine/Engine.hpp"
#include "../maths/vector3.hpp"
#include "../sounds/Sound.hpp"

#include "Display.hpp"

namespace Flounder
{
	typedef struct SoundSourceInfo
	{
		unsigned int size;
		unsigned char *data;
		short formatTag;
		short channels;
		int samplesPerSec;
		int averageBytesPerSec;
		short blockAlign;
		short bitsPerSample;
	} SoundSourceInfo;

	/// <summary>
	/// A module used for loading, managing and playing a variety of different sound types.
	/// </summary>
	class Audio :
		public IModule
	{
	private:
		friend class Sound;

		static std::vector<Sound*> m_sounds;

		ALCdevice *m_alDevice;
		ALCcontext *m_alContext;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline Audio *Get()
		{
			return static_cast<Audio*>(Engine::Get()->GetModule("audio"));
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

		static SoundSourceInfo LoadFileWav(const std::string &path);

		static Sound *AddSound(Sound *object);

		static Sound *GetSound(const std::string &name);
	};
}
