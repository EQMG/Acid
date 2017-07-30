#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../helpers/helperstring.h"

#ifdef FLOUNDER_PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
//#include <AL/al.h>
//#include <AL/alc.h>
#endif

namespace flounder
{
	class vector3;

	class sound
	{
	private:
		std::string m_name;
		std::string m_filename;
		unsigned int m_count;

#ifndef FLOUNDER_PLATFORM_WEB
		unsigned int m_buffer;
		unsigned int m_source;
#endif

		bool m_playing;
		float m_pitch;
		float m_gain;
	public:
		sound(const std::string &name, const std::string &filename);

		~sound();

		void play();

		void loop();

		void pause();

		void resume();

		void stop();

		void setPosition(const float &x, const float &y, const float &z);

		void setPosition(const vector3 &position);

		void setDirection(const float &x, const float &y, const float &z);

		void setDirection(const vector3 &direction);

		void setVelocity(const float &x, const float &y, const float &z);;

		void setVelocity(const vector3 &velocity);

		void setPitch(float pitch);

		void setGain(float gain);

		inline std::string getName() const { return m_name; }

		inline std::string getFileName() const { return m_filename; }

		inline bool isPlaying() const { return m_playing; }

		inline float getPitch() const { return m_pitch; }

		inline float getGain() const { return m_gain; }
	};
}
