#include "audio.h"

namespace flounder
{
	std::vector<sound*> audio::m_sounds = std::vector<sound*>();

	audio::audio() :
		imodule()
	{
#ifdef FLOUNDER_PLATFORM_WEB
		EM_ASM(
			Module.audio = {};
			Module.audio.m_sounds = {};
			Module.audioAdd = function(name, filename)
			{
				console.log("Adding audio: " + name);
				Module.audio.m_sounds[name] = new Audio(filename);
			};
			Module.audioPlay = function(name)
			{
				console.log("Playing audio: " + name);
				Module.audio.m_sounds[name].play();
			};
			Module.audioPause = function(name)
			{
				console.log("Pausing audio: " + name);
				Module.audio.m_sounds[name].pause();
			};
			Module.audioStop = function(name)
			{
				console.log("Stopping audio: " + name);
				Module.audioPause(name);
				Module.audio.m_sounds[name].currentTime = 0;
				Module.audio.m_sounds[name].loop = false;
			};
			Module.audioLoop = function(name)
			{
				console.log("Looping audio: " + name);
				Module.audio.m_sounds[name].play();
				Module.audio.m_sounds[name].loop = true;
			};
			Module.audioSetGain = function(name, gain)
			{
				console.log("Setting gain to audio: " + name);
				Module.audio.m_sounds[name].volume = gain;
			};
		);
#else
		gc_initialize(0);
		m_manager = gau_manager_create();
		m_mixer = gau_manager_mixer(m_manager);
#endif
	}

	audio::~audio()
	{
		for (int i = 0; i < m_sounds.size(); i++)
		{
			delete m_sounds[i];
		}

#ifndef FLOUNDER_PLATFORM_WEB
		gau_manager_destroy(m_manager);
		gc_shutdown();
#endif
	}

	void audio::update()
	{
#ifndef FLOUNDER_PLATFORM_WEB
		gau_manager_update(m_manager);
#endif
	}

	sound *audio::add(sound *object)
	{
		m_sounds.push_back(object);
#ifdef FLOUNDER_PLATFORM_WEB
		audioAdd(object->getName().c_str(), object->getFileName().c_str());
#endif
		return object;
	}

	sound *audio::get(const std::string &name)
	{
		for (sound *object : m_sounds)
		{
			if (object->getName() == name)
			{
				return object;
			}
		}

		return NULL;
	}
}
