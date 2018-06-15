#include "Sound.hpp"

#include <cmath>
#ifdef FL_BUILD_MACOS
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include "SoundBuffer.hpp"

namespace fl
{
	Sound::Sound(const std::string &filename, const float &gain, const float &pitch) :
		m_source(0),
		m_playing(false),
		m_gain(gain),
		m_pitch(pitch)
	{
		auto soundBuffer = SoundBuffer::Resource(filename);

		alGenSources(1, &m_source);
		alSourcei(m_source, AL_BUFFER, soundBuffer->GetBuffer());

		Audio::ErrorAl(alGetError());

		SetGain(gain);
		SetPitch(pitch);
	}

	Sound::~Sound()
	{
		alDeleteSources(1, &m_source);
		Audio::ErrorAl(alGetError());
	}

	void Sound::Play(const bool &loop)
	{
		alSourcei(m_source, AL_LOOPING, loop);
		alSourcePlay(m_source);
		m_playing = true;
		Audio::ErrorAl(alGetError());
	}

	void Sound::Pause()
	{
		if (!m_playing)
		{
			return;
		}

		alSourcePause(m_source);
		m_playing = false;
		Audio::ErrorAl(alGetError());
	}

	void Sound::Resume()
	{
		if (m_playing)
		{
			return;
		}

		alSourcei(m_source, AL_LOOPING, false);
		alSourcePlay(m_source);
		m_playing = true;
		Audio::ErrorAl(alGetError());
	}

	void Sound::Stop()
	{
		if (!m_playing)
		{
			return;
		}

		alSourceStop(m_source);
		m_playing = false;
		Audio::ErrorAl(alGetError());
	}

	void Sound::SetPosition(const Vector3 &position)
	{
		alSource3f(m_source, AL_POSITION, position.m_x, position.m_y, position.m_z);
		Audio::ErrorAl(alGetError());
	}

	void Sound::SetDirection(const Vector3 &direction)
	{
		float data[3] = {direction.m_x, direction.m_y, direction.m_z};
		alSourcefv(m_source, AL_DIRECTION, data);
		Audio::ErrorAl(alGetError());
	}

	void Sound::SetVelocity(const Vector3 &velocity)
	{
		alSource3f(m_source, AL_VELOCITY, velocity.m_x, velocity.m_y, velocity.m_z);
		Audio::ErrorAl(alGetError());
	}

	void Sound::SetGain(const float &gain)
	{
		float eulerGain = std::pow(gain, 2.7183f);
		alSourcef(m_source, AL_GAIN, eulerGain);
		m_gain = eulerGain;
		Audio::ErrorAl(alGetError());
	}

	void Sound::SetPitch(const float &pitch)
	{
		alSourcef(m_source, AL_PITCH, pitch);
		m_pitch = pitch;
	}
}
