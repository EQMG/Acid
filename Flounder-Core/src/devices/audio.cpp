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
#endif
	}

	audio::~audio()
	{
		for (int i = 0; i < m_sounds->size(); i++)
		{
		//	delete (*m_sounds)[i];
		}

		delete m_sounds;

#ifdef FLOUNDER_PLATFORM_WEB
#endif
	}

	void audio::update()
	{

#ifdef FLOUNDER_PLATFORM_WEB
#endif
	}

	sound *audio::add(sound *object)
	{
		audio::get()->m_sounds->push_back(object);
#ifdef FLOUNDER_PLATFORM_WEB
		SoundManagerAdd(object->getName().c_str(), object->getFileName().c_str());
#endif
		return object;
	}

	sound *audio::get(const std::string & name)
	{
		for (sound* object : *audio::get()->m_sounds)
		{
			if (object->getName() == name)
			{
				return object;
			}
		}

		return NULL;
	}
}
