#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../helpers/helperstring.h"

#ifdef FLOUNDER_PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#include <gorilla/ga.h>
#include <gorilla/gau.h>
#endif

namespace flounder
{
	class sound
	{
	private:
		std::string m_name;
		std::string m_filename;
		unsigned int m_count;
#ifndef FLOUNDER_PLATFORM_WEB
		ga_Sound* m_sound;
		ga_Handle* m_handle;
		gc_int32 m_position;
#endif
		bool m_playing;
		float m_gain;
	public:
		sound(const std::string& name, const std::string& filename);
		
		~sound();

		void play();

		void loop();

		void pause();

		void resume();

		void stop();

		void setGain(float gain);

		inline std::string getName() const { return m_name; }

		inline std::string getFileName() const { return m_filename; }

		inline bool isPlaying() const { return m_playing; }

		inline float getGain() const { return m_gain; }

#ifndef FLOUNDER_PLATFORM_WEB
		friend void destroy_on_finish(ga_Handle* in_handle, void* in_context);

		friend void loop_on_finish(ga_Handle* in_handle, void* in_context);
#endif
	};
}
