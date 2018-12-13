﻿#include "Sound.hpp"

#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#else
#include <al.h>
#endif
#include <cmath>
#include "Objects/GameObject.hpp"

namespace acid
{
	Sound::Sound(const std::string &filename, const SoundType &type, const bool &begin, const bool &loop, const float &gain, const float &pitch) :
		m_soundBuffer(SoundBuffer::Resource(filename)),
		m_source(0),
		m_type(type),
		m_gain(gain),
		m_pitch(pitch),
		m_position(Vector3()),
		m_direction(Vector3()),
		m_velocity(Vector3())
	{
		alGenSources(1, &m_source);
		alSourcei(m_source, AL_BUFFER, m_soundBuffer->GetBuffer());

		Audio::CheckAl(alGetError());

		SetGain(gain);
		SetPitch(pitch);

		if (begin)
		{
			Play(loop);
		}
	}

	Sound::~Sound()
	{
		alDeleteSources(1, &m_source);
		Audio::CheckAl(alGetError());
	}

	void Sound::Start()
	{
	}

	void Sound::Update()
	{
		auto transform = GetGameObject()->GetWorldTransform();
		SetPosition(transform.GetPosition());
	}

	void Sound::Decode(const Metadata &metadata)
	{
		m_soundBuffer = SoundBuffer::Resource(metadata.GetChild<std::string>("Filename"));
	}

	void Sound::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Filename", m_soundBuffer == nullptr ? "" : m_soundBuffer->GetFilename());
	}

	void Sound::Play(const bool &loop)
	{
		alSourcei(m_source, AL_LOOPING, loop);
		alSourcePlay(m_source);
		Audio::CheckAl(alGetError());

		SetGain(m_gain);
	}

	void Sound::Pause()
	{
		if (!IsPlaying())
		{
			return;
		}

		alSourcePause(m_source);
		Audio::CheckAl(alGetError());
	}

	void Sound::Resume()
	{
		if (IsPlaying())
		{
			return;
		}

		alSourcePlay(m_source);
		Audio::CheckAl(alGetError());

		SetGain(m_gain);
	}

	void Sound::Stop()
	{
		if (!IsPlaying())
		{
			return;
		}

		alSourceStop(m_source);
		Audio::CheckAl(alGetError());
	}

	bool Sound::IsPlaying()
	{
		ALenum state;
		alGetSourcei(m_source, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}

	void Sound::SetPosition(const Vector3 &position)
	{
		m_position = position;
		alSource3f(m_source, AL_POSITION, m_position.m_x, m_position.m_y, m_position.m_z);
		Audio::CheckAl(alGetError());
	}

	void Sound::SetDirection(const Vector3 &direction)
	{
		m_direction = direction;
		alSourcefv(m_source, AL_DIRECTION, m_direction.m_elements);
		Audio::CheckAl(alGetError());
	}

	void Sound::SetVelocity(const Vector3 &velocity)
	{
		m_velocity = velocity;
		alSource3f(m_source, AL_VELOCITY, m_velocity.m_x, m_velocity.m_y, m_velocity.m_z);
		Audio::CheckAl(alGetError());
	}

	void Sound::SetGain(const float &gain)
	{
		m_gain = gain;
		alSourcef(m_source, AL_GAIN, std::pow(m_gain * Audio::Get()->GetTypeGain(m_type), 2.7183f));
		Audio::CheckAl(alGetError());
	}

	void Sound::SetPitch(const float &pitch)
	{
		m_pitch = pitch;
		alSourcef(m_source, AL_PITCH, m_pitch);
		Audio::CheckAl(alGetError());
	}
}
