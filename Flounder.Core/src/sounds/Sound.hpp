#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <al/al.h>
#include <al/alc.h>

#include "../devices/Display.hpp"
#include "../helpers/HelperString.hpp"
#include "../maths/Vector3.hpp"

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

		void SetPosition(const Vector3 &position);

		void SetDirection(const Vector3 &direction);

		void SetVelocity(const Vector3 &velocity);

		void SetPitch(float pitch);

		void SetGain(float gain);

		std::string GetName() const { return m_name; }

		std::string GetFileName() const { return m_filename; }

		bool IsPlaying() const { return m_playing; }

		float GetPitch() const { return m_pitch; }

		float GetGain() const { return m_gain; }
	};
}
