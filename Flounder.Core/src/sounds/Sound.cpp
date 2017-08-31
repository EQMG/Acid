#include "Sound.hpp"

#include "../devices/Audio.hpp"

namespace Flounder
{
	// TODO: https://github.com/AndySmile/SimpleAudioLibrary

	Sound::Sound(const std::string &name, const std::string &filename) :
		m_name(name),
		m_filename(filename),
		m_count(0),
		m_buffer(0),
		m_source(0),
		m_playing(false),
		m_pitch(1.0f),
		m_gain(1.0f)
	{
		SoundSourceInfo sourceInfo = Audio::LoadFileWav(filename);

		alGenBuffers(1, &m_buffer);
		alBufferData(m_buffer, (sourceInfo.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, sourceInfo.data, sourceInfo.size, sourceInfo.samplesPerSec);

		alGenSources(1, &m_source);
		alSourcei(m_source, AL_BUFFER, m_buffer);

		delete[] sourceInfo.data;
	}

	Sound::~Sound()
	{
		alDeleteSources(1, &m_source);
		alDeleteBuffers(1, &m_buffer);
	}

	void Sound::Play()
	{
		alSourcei(m_source, AL_LOOPING, false);
		alSourcePlay(m_source);
		m_playing = true;
	}

	void Sound::Loop()
	{
		alSourcei(m_source, AL_LOOPING, true);
		alSourcePlay(m_source);
		m_playing = true;
	}

	void Sound::Pause()
	{
		if (!m_playing)
		{
			return;
		}

		alSourcePause(m_source);
		m_playing = false;
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
	}

	void Sound::Stop()
	{
		if (!m_playing)
		{
			return;
		}

		alSourceStop(m_source);
		m_playing = false;
	}

	void Sound::SetPosition(const Vector3 &position)
	{
		alSource3f(m_source, AL_POSITION, position.m_x, position.m_y, position.m_z);
	}

	void Sound::SetDirection(const Vector3 &direction)
	{
		float data[3] = {direction.m_x, direction.m_y, direction.m_z};
		alSourcefv(m_source, AL_DIRECTION, data);
	}

	void Sound::SetVelocity(const Vector3 &velocity)
	{
		alSource3f(m_source, AL_VELOCITY, velocity.m_x, velocity.m_y, velocity.m_z);
	}

	void Sound::SetPitch(float pitch)
	{
		alSourcef(m_source, AL_PITCH, pitch);
		m_pitch = pitch;
	}

	void Sound::SetGain(float gain)
	{
		alSourcef(m_source, AL_GAIN, gain);
		m_gain = gain;
	}
}
