#pragma once

#include <iostream>
#include <vector>
#include <map>

#ifdef FLOUNDER_PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#endif

#include "../camera/camera.h"
#include "../framework/framework.h"
#include "../maths/vector3.h"
#include "../sounds/sound.h"

#include "display.h"

#ifdef FLOUNDER_PLATFORM_WEB
extern "C" void audioAdd(const char* name, const char* filename);
extern "C" void audioPlay(const char* name);
extern "C" void audioPause(const char* name);
extern "C" void audioStop(const char* name);
extern "C" void audioLoop(const char* name);
extern "C" void audioSetPitch(const char* name, double pitch);
extern "C" void audioSetGain(const char* name, double gain);
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
#endif

		static std::vector<sound*> m_sounds;
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

		static sound *add(sound *object);

		static sound *get(const std::string &name);
	};
}
