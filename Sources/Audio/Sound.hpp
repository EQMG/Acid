#pragma once

#include <string>
#include "Audio/Audio.hpp"
#include "Maths/Vector3.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a loaded sound.
	/// </summary>
	class FL_EXPORT Sound
	{
	private:
		ALuint m_source;

		bool m_playing;
		float m_gain;
		float m_pitch;
	public:
		Sound(const std::string &filename, const float &gain = 1.0f, const float &pitch = 1.0f);

		~Sound();

		void Play();

		void Loop();

		void Pause();

		void Resume();

		void Stop();

		void SetPosition(const Vector3 &position);

		void SetDirection(const Vector3 &direction);

		void SetVelocity(const Vector3 &velocity);

		bool IsPlaying() const { return m_playing; }

		float GetGain() const { return m_gain; }

		void SetGain(const float &gain);

		float GetPitch() const { return m_pitch; }

		void SetPitch(const float &pitch);
	};
}
