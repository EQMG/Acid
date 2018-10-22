#pragma once

#include <string>
#include "Maths/Vector3.hpp"
#include "Objects/IComponent.hpp"
#include "SoundBuffer.hpp"
#include "Audio.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a loaded sound.
	/// </summary>
	class ACID_EXPORT Sound :
		public IComponent
	{
	private:
		std::shared_ptr<SoundBuffer> m_soundBuffer;
		uint32_t m_source;

		SoundType m_type;
		float m_gain;
		float m_pitch;

		Vector3 m_position;
		Vector3 m_direction;
		Vector3 m_velocity;
	public:
		explicit Sound(const std::string &filename, const SoundType &type = SOUND_TYPE_GENERAL, const bool &begin = false, const bool &loop = false, const float &gain = 1.0f, const float &pitch = 1.0f);

		~Sound();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void Play(const bool &loop = false);

		void Pause();

		void Resume();

		void Stop();

		bool IsPlaying();

		void SetPosition(const Vector3 &position);

		void SetDirection(const Vector3 &direction);

		void SetVelocity(const Vector3 &velocity);

		SoundType GetType() const { return m_type; }

		void SetType(const SoundType &type) { m_type = type; }

		float GetGain() const { return m_gain; }

		void SetGain(const float &gain);

		float GetPitch() const { return m_pitch; }

		void SetPitch(const float &pitch);
	};
}
