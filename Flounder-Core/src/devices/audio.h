#pragma once

#include <vector>

#ifdef FLOUNDER_PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#define GAU_THREAD_POLICY_MULTI 2
#include <gorilla/ga.h>
#include <gorilla/gau.h>
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

#ifndef FLOUNDER_PLATFORM_WEB
		static gau_Manager *m_manager;
		static ga_Mixer *m_mixer;
#endif

		static std::vector<sound*> *m_sounds;
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

#ifndef FLOUNDER_PLATFORM_WEB
		static gau_Manager * getManager() { return m_manager; }

		static ga_Mixer * getMixer() { return m_mixer; }
#endif
	};
}
