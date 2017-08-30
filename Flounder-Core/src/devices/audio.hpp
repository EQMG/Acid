#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <map>

#include <al/al.h>
#include <al/alc.h>

#include "../camera/camera.hpp"
#include "../framework/framework.hpp"
#include "../maths/vector3.hpp"
#include "../sounds/sound.hpp"

#include "display.hpp"

namespace flounder
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
	class audio :
		public imodule
	{
	private:
		friend class sound;

		static std::vector<sound*> m_sounds;

		ALCdevice *m_device;
		ALCcontext *m_context;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline audio *get()
		{
			return static_cast<audio*>(framework::get()->getInstance("audio"));
		}

		/// <summary>
		/// Creates a new audio module.
		/// </summary>
		audio();

		/// <summary>
		/// Deconstructor for the audio module.
		/// </summary>
		~audio();

		void update() override;

		static SoundSourceInfo loadWaveFile(const std::string &path);

		static sound *add(sound *object);

		static sound *get(const std::string &name);
	};
}
