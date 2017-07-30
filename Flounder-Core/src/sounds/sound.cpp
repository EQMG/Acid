#include "sound.h"
#include "../devices/audio.h"

namespace flounder
{
	sound::sound(const std::string &name, const std::string &filename)
	{
		m_name = name;
		m_filename = filename;
		m_count = 0;

		m_playing = false;

		std::vector<std::string> split = helperstring::split(m_filename, ".");

#ifndef FLOUNDER_PLATFORM_WEB
		m_sound = gau_load_sound_file(filename.c_str(), split.back().c_str());
#endif
	}

	sound::~sound()
	{
#ifndef FLOUNDER_PLATFORM_WEB
		ga_sound_release(m_sound);
#endif
	}

	void sound::play()
	{
#ifdef FLOUNDER_PLATFORM_WEB
		audioPlay(m_name.c_str());
#else
		gc_int32 quit = 0;
		m_handle = gau_create_handle_sound(audio::m_mixer, m_sound, &destroy_on_finish, &quit, NULL);
		m_handle->sound = this;
		ga_handle_play(m_handle);
		m_count++;
#endif
		m_playing = true;
	}

	void sound::loop()
	{
#ifdef FLOUNDER_PLATFORM_WEB
		audioLoop(m_name.c_str());
#else
		gc_int32 quit = 0;
		m_handle = gau_create_handle_sound(audio::m_mixer, m_sound, &loop_on_finish, &quit, NULL);
		m_handle->sound = this;
		ga_handle_play(m_handle);
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
		ga_handle_play(m_handle);
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
		ga_handle_stop(m_handle);
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
		ga_handle_stop(m_handle);
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
#else
		ga_handle_setParamf(m_handle, GA_HANDLE_PARAM_GAIN, gain);
#endif
	}

#ifndef FLOUNDER_PLATFORM_WEB
	void destroy_on_finish(ga_Handle* in_handle, void* in_context)
	{
		sound* object = static_cast<sound*>(in_handle->sound);
		object->m_count--;

		if (object->m_count == 0)
		{
			object->stop();
		}
	}

	void loop_on_finish(ga_Handle* in_handle, void* in_context)
	{
		sound* object = static_cast<sound*>(in_handle->sound);
		object->loop();
		ga_handle_destroy(in_handle);
	}
#endif
}
