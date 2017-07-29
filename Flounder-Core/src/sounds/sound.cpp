#include "sound.h"
#include "../devices/audio.h"

namespace flounder
{
	sound::sound(const std::string &name, const std::string &filename)
	{
		m_name = name;
		m_filename = filename;
		m_count = 0;

		std::vector<std::string> split = helperstring::split(m_filename, ".");

		if (split.size() < 2)
		{
			std::cout << "Invalid sound file name: " << m_filename << std::endl;
			return;
		}

		m_playing = false;
	}

	sound::~sound()
	{
	}

	void sound::play()
	{
#ifdef FLOUNDER_PLATFORM_WEB
		audioPlay(m_name.c_str());
#endif
		m_playing = true;
	}

	void sound::loop()
	{
#ifdef FLOUNDER_PLATFORM_WEB
		audioLoop(m_name.c_str());
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
#endif
		m_playing = false;
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
#endif
	}
}
