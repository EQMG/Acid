#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <al/al.h>
#include <al/alc.h>

#include "../devices/Display.hpp"
#include "../helpers/helperstring.hpp"
#include "../maths/vector3.hpp"

namespace Flounder
{
	class Sound
	{
	private:
		std::string m_name;
		std::string m_filename;
		unsigned int m_count;

		unsigned int m_buffer;
		unsigned int m_source;

		bool m_playing;
		float m_pitch;
		float m_gain;
	public:
		Sound(const std::string &name, const std::string &filename);

		~Sound();

		void Play();

		void Loop();

		void Pause();

		void Resume();

		void Stop();

		void SetPosition(const vector3 &position);

		void SetDirection(const vector3 &direction);

		void SetVelocity(const vector3 &velocity);

		void SetPitch(float pitch);

		void SetGain(float gain);

		inline std::string GetName() const { return m_name; }

		inline std::string GetFileName() const { return m_filename; }

		inline bool IsPlaying() const { return m_playing; }

		inline float GetPitch() const { return m_pitch; }

		inline float GetGain() const { return m_gain; }
	};
}
