#pragma once

#include <iostream>
#include <vector>

#ifdef FLOUNDER_PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

#include "../camera/camera.h"
#include "../framework/framework.h"
#include "../maths/vector3.h"
#include "sound.h"

#include "display.h"

#ifdef FLOUNDER_PLATFORM_WEB
extern "C" void SoundManagerAdd(const char *name, const char *filename);
extern "C" void SoundManagerPlay(const char *name);
extern "C" void SoundManagerPause(const char *name);
extern "C" void SoundManagerStop(const char *name);
extern "C" void SoundManagerLoop(const char *name);
extern "C" void SoundManagerSetGain(const char* name, double gain);
#endif

namespace flounder
{
	/// <summary>
	/// A module used for loading, managing and playing a variety of different sound types.
	/// </summary>
	class audio :
		public imodule
	{
	private:
		friend class sound;

		std::vector<sound*> *m_sounds;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static audio *get()
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

		static sound* add(sound* object);

		static sound* get(const std::string& name);
	};
}
