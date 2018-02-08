#pragma once

#include <string>
#include "../Maths/Vector3.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a loaded sound.
	/// </summary>
	class Sound
	{
	private:
		unsigned int m_source;

		bool m_playing;
		float m_pitch;
		float m_gain;
	public:
		Sound(const std::string &filename);

		~Sound();

		void Play();

		void Loop();

		void Pause();

		void Resume();

		void Stop();

		void SetPosition(const Vector3 &position);

		void SetDirection(const Vector3 &direction);

		void SetVelocity(const Vector3 &velocity);

		void SetPitch(const float &pitch);

		void SetGain(const float &gain);

		bool IsPlaying() const { return m_playing; }

		float GetPitch() const { return m_pitch; }

		float GetGain() const { return m_gain; }
	};
}
