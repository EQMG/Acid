#include "sound.h"
#include "../devices/audio.h"

namespace flounder
{
	// TODO: Desktop: https://github.com/AndySmile/SimpleAudioLibrary
	sound::sound(const std::string &name, const std::string &filename)
	{
		m_name = name;
		m_filename = filename;
		m_count = 0;

		m_playing = false;
		m_pitch = 1.0f;
		m_gain = 1.0f;

#ifndef FLOUNDER_PLATFORM_WEB
		//unsigned char **data = NULL;
		//unsigned int *size = NULL;
		//unsigned int *frequency = NULL;
		//short *numChannels = NULL;

		//audio::loadWaveFile(filename, data, size, frequency, numChannels);

		//alGenBuffers(1, &m_buffer);
		//alBufferData(m_buffer, (*numChannels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, *data, *size, *frequency);

		//alGenSources(1, &m_source);
		//alSourcei(m_source, AL_BUFFER, m_buffer);
#endif
	}

	sound::~sound()
	{
#ifndef FLOUNDER_PLATFORM_WEB
#endif
	}

	void sound::play()
	{
#ifdef FLOUNDER_PLATFORM_WEB
		audioPlay(m_name.c_str());
#else
		//alSourcei(m_source, AL_LOOPING, false);
		//alSourcePlay(m_source);
#endif
		m_playing = true;
	}

	void sound::loop()
	{
#ifdef FLOUNDER_PLATFORM_WEB
		audioLoop(m_name.c_str());
#else
		//alSourcei(m_source, AL_LOOPING, true);
		//alSourcePlay(m_source);
#endif
		m_playing = true;
	}

	void sound::pause()
	{
		if (!m_playing)
		{
			return;
		}

		m_playing = false;
#ifdef FLOUNDER_PLATFORM_WEB
		audioPause(m_name.c_str());
#else
		//alSourcePause(m_source);
#endif
	}

	void sound::resume()
	{
		if (m_playing)
		{
			return;
		}

		m_playing = true;
#ifdef FLOUNDER_PLATFORM_WEB
		audioPlay(m_name.c_str());
#else
		//alSourcei(m_source, AL_LOOPING, false);
		//alSourcePlay(m_source);
#endif
	}

	void sound::stop()
	{
		if (!m_playing)
		{
			return;
		}

#ifdef FLOUNDER_PLATFORM_WEB
		audioStop(m_name.c_str());
#else
		//alSourceStop(m_source);
#endif
		m_playing = false;
	}

	void sound::setPosition(const float &x, const float &y, const float &z)
	{
#ifdef FLOUNDER_PLATFORM_WEB
#else
		//alSource3f(m_source, AL_POSITION, x, y, z);
#endif
	}

	void sound::setPosition(const vector3 &position)
	{
		setPosition(position.m_x, position.m_y, position.m_z);
	}

	void sound::setDirection(const float &x, const float &y, const float &z)
	{
#ifdef FLOUNDER_PLATFORM_WEB
#else
		//float direction[3] = { x, y, z };
		//alSourcefv(m_source, AL_DIRECTION, direction);
#endif
	}

	void sound::setDirection(const vector3 &direction)
	{
		setDirection(direction.m_x, direction.m_y, direction.m_z);
	}

	void sound::setVelocity(const float &x, const float &y, const float &z)
	{
#ifdef FLOUNDER_PLATFORM_WEB
#else
		//alSource3f(m_source, AL_VELOCITY, x, y, z);
#endif
	}

	void sound::setVelocity(const vector3 &velocity)
	{
		setVelocity(velocity.m_x, velocity.m_y, velocity.m_z);
	}

	void sound::setPitch(float pitch)
	{
		if (!m_playing)
		{
			std::cout << "Cannot set sound pitch! Sound is not currently playing!" << std::endl;
			return;
		}

		m_pitch = pitch;
#ifdef FLOUNDER_PLATFORM_WEB
		audioSetPitch(m_name.c_str(), m_pitch);
#else
		//alSourcef(m_source, AL_PITCH, m_pitch);
#endif
	}

	void sound::setGain(float gain)
	{
		if (!m_playing)
		{
			std::cout << "Cannot set sound gain! Sound is not currently playing!" << std::endl;
			return;
		}

		m_gain = gain;
#ifdef FLOUNDER_PLATFORM_WEB
		audioSetGain(m_name.c_str(), m_gain);
#else
		//alSourcef(m_source, AL_GAIN, m_gain);
#endif
	}
}
