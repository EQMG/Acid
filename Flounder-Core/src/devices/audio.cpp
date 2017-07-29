#include "audio.h"

namespace flounder
{
	audio::audio() :
		imodule()
	{
		m_sounds = new std::vector<sound*>();

#ifdef FLOUNDER_PLATFORM_WEB
		EM_ASM(
		Module.SoundManager = {};
		Module.SoundManager.m_sounds = {};
		Module.SoundManagerAdd = function(name, filename) { Module.SoundManager.m_sounds[name] = new Audio(filename); };
		Module.SoundManagerPlay = function(name) { Module.SoundManager.m_sounds[name].play(); };
		Module.SoundManagerPause = function(name) { Module.SoundManager.m_sounds[name].pause(); };
		Module.SoundManagerStop = function(name) { Module.SoundManagerPause(name); Module.SoundManager.m_sounds[name].currentTime = 0; Module.SoundManager.m_sounds[name].loop = false; };
		Module.SoundManagerLoop = function(name) { Module.SoundManager.m_sounds[name].play(); Module.SoundManager.m_sounds[name].loop = true; };
		Module.SoundManagerSetGain = function(name, gain) { Module.SoundManager.m_sounds[name].volume = gain; };
		);
#else
		gc_initialize(0);
		m_manager = gau_manager_create();
		m_mixer = gau_manager_mixer(m_manager);
#endif
	}

	audio::~audio()
	{
		for (int i = 0; i < m_sounds->size(); i++)
		{
			delete (*m_sounds)[i];
		}

#ifdef FLOUNDER_PLATFORM_WEB
#else
		gau_manager_destroy(m_manager);
		gc_shutdown();
#endif
	}

	void audio::update()
	{

#ifdef FLOUNDER_PLATFORM_WEB
#else
		gau_manager_update(m_manager);
#endif
	}

	sound *audio::add(sound *object)
	{
		m_sounds->push_back(object);
#ifdef FLOUNDER_PLATFORM_WEB
		SoundManagerAdd(object->getName().c_str(), object->getFileName().c_str());
#endif
		return object;
	}

	sound *audio::get(const std::string & name)
	{
		for (sound* sound : *m_sounds)
		{
			if (sound->getName() == name)
			{
				return sound;
			}
		}

		return NULL;
	}
}
