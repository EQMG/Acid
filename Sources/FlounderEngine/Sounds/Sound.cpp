#include "Sound.hpp"

#include "../Devices/Audio.hpp"
#include "SoundBuffer.hpp"

namespace Flounder
{
	Sound::Sound(const std::string &filename) :
		m_source(0),
		m_playing(false),
		m_pitch(1.0f),
		m_gain(1.0f)
	{
		SoundBuffer *soundBuffer = SoundBuffer::Resource(filename);

		alGenSources(1, &m_source);
		alSourcei(m_source, AL_BUFFER, soundBuffer->GetBuffer());

		Platform::ErrorAl(alGetError());
	}

	Sound::~Sound()
	{
		alDeleteSources(1, &m_source);
		Platform::ErrorAl(alGetError());
	}

	void Sound::Play()
	{
		alSourcei(m_source, AL_LOOPING, false);
		alSourcePlay(m_source);
		m_playing = true;
		Platform::ErrorAl(alGetError());
	}

	void Sound::Loop()
	{
		alSourcei(m_source, AL_LOOPING, true);
		alSourcePlay(m_source);
		m_playing = true;
		Platform::ErrorAl(alGetError());
	}

	void Sound::Pause()
	{
		if (!m_playing)
		{
			return;
		}

		alSourcePause(m_source);
		m_playing = false;
		Platform::ErrorAl(alGetError());
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
		Platform::ErrorAl(alGetError());
	}

	void Sound::Stop()
	{
		if (!m_playing)
		{
			return;
		}

		alSourceStop(m_source);
		m_playing = false;
		Platform::ErrorAl(alGetError());
	}

	void Sound::SetPosition(const Vector3 &position)
	{
		alSource3f(m_source, AL_POSITION, position.m_x, position.m_y, position.m_z);
		Platform::ErrorAl(alGetError());
	}

	void Sound::SetDirection(const Vector3 &direction)
	{
		float data[3] = {direction.m_x, direction.m_y, direction.m_z};
		alSourcefv(m_source, AL_DIRECTION, data);
		Platform::ErrorAl(alGetError());
	}

	void Sound::SetVelocity(const Vector3 &velocity)
	{
		alSource3f(m_source, AL_VELOCITY, velocity.m_x, velocity.m_y, velocity.m_z);
		Platform::ErrorAl(alGetError());
	}

	void Sound::SetPitch(const float &pitch)
	{
		alSourcef(m_source, AL_PITCH, pitch);
		m_pitch = pitch;
	}

	void Sound::SetGain(const float &gain)
	{
		alSourcef(m_source, AL_GAIN, gain);
		m_gain = gain;
		Platform::ErrorAl(alGetError());
	}
}
