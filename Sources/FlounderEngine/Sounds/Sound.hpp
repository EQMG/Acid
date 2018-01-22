#pragma once

#include <string>
#include <vector>
#include "../Resources/Resources.hpp"
#include "../Maths/Vector3.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a loaded sound.
	/// </summary>
	class Sound :
		public IResource
	{
	private:
		std::string m_filename;
		unsigned int m_count;

		unsigned int m_buffer;
		unsigned int m_source;

		bool m_playing;
		float m_pitch;
		float m_gain;
	public:
		static Sound *Resource(const std::string &filename)
		{
			IResource *resource = Resources::Get()->Get(filename);

			if (resource != nullptr)
			{
				return dynamic_cast<Sound *>(resource);
			}

			Sound *result = new Sound(filename);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

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

		std::string GetFilename() override { return m_filename; };

		bool IsPlaying() const { return m_playing; }

		float GetPitch() const { return m_pitch; }

		float GetGain() const { return m_gain; }
	};
}
