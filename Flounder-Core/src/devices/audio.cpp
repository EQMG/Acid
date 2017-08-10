#include "audio.h"

namespace flounder
{
	std::vector<sound*> audio::m_sounds = std::vector<sound*>();

	audio::audio() :
		imodule()
	{
	}

	audio::~audio()
	{
		for (int i = 0; i < m_sounds.size(); i++)
		{
			delete m_sounds[i];
		}
	}

	void audio::update()
	{
	}

	sound *audio::add(sound *object)
	{
		m_sounds.push_back(object);
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

		return nullptr;
	}
}
