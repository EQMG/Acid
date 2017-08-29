#include "sound.hpp"
#include "../devices/audio.hpp"

namespace flounder
{
	// TODO: https://github.com/AndySmile/SimpleAudioLibrary

	sound::sound(const std::string &name, const std::string &filename)
	{
		m_name = name;
		m_filename = filename;
		m_count = 0;

		m_buffer = 0;
		m_source = 0;

		m_playing = false;
		m_pitch = 1.0f;
		m_gain = 1.0f;

		SoundSourceInfo sourceInfo = audio::loadWaveFile(filename);

		alGenBuffers(1, &m_buffer);
		alBufferData(m_buffer, (sourceInfo.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, sourceInfo.data, sourceInfo.size, sourceInfo.samplesPerSec);

		alGenSources(1, &m_source);
		alSourcei(m_source, AL_BUFFER, m_buffer);

		delete[] sourceInfo.data;
	}

	sound::~sound()
	{
		alDeleteSources(1, &m_source);
		alDeleteBuffers(1, &m_buffer);
	}

	void sound::play()
	{
		alSourcei(m_source, AL_LOOPING, false);
		alSourcePlay(m_source);
		m_playing = true;
	}

	void sound::loop()
	{
		alSourcei(m_source, AL_LOOPING, true);
		alSourcePlay(m_source);
		m_playing = true;
	}

	void sound::pause()
	{
		if (!m_playing)
		{
			return;
		}

		alSourcePause(m_source);
		m_playing = false;
	}

	void sound::resume()
	{
		if (m_playing)
		{
			return;
		}

		alSourcei(m_source, AL_LOOPING, false);
		alSourcePlay(m_source);
		m_playing = true;
	}

	void sound::stop()
	{
		if (!m_playing)
		{
			return;
		}

		alSourceStop(m_source);
		m_playing = false;
	}

	void sound::setPosition(const float &x, const float &y, const float &z)
	{
		alSource3f(m_source, AL_POSITION, x, y, z);
	}

	void sound::setPosition(const vector3 &position)
	{
		setPosition(position.m_x, position.m_y, position.m_z);
	}

	void sound::setDirection(const float &x, const float &y, const float &z)
	{
		float direction[3] = {x, y, z};
		alSourcefv(m_source, AL_DIRECTION, direction);
	}

	void sound::setDirection(const vector3 &direction)
	{
		setDirection(direction.m_x, direction.m_y, direction.m_z);
	}

	void sound::setVelocity(const float &x, const float &y, const float &z)
	{
		alSource3f(m_source, AL_VELOCITY, x, y, z);
	}

	void sound::setVelocity(const vector3 &velocity)
	{
		setVelocity(velocity.m_x, velocity.m_y, velocity.m_z);
	}

	void sound::setPitch(float pitch)
	{
		alSourcef(m_source, AL_PITCH, pitch);
		m_pitch = pitch;
	}

	void sound::setGain(float gain)
	{
		alSourcef(m_source, AL_GAIN, gain);
		m_gain = gain;
	}
}
