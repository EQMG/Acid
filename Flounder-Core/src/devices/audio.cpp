#include "audio.h"

namespace flounder
{
	audio::audio() :
		imodule()
	{
		// Creates the OpenAL contexts.
		m_device = alcOpenDevice(NULL);

		m_context = alcCreateContext(m_device, NULL);
		ALCboolean made = alcMakeContextCurrent(m_context);

		// Checks for errors.
		ALenum alError = alGetError();

		if (alError != GL_NO_ERROR)
		{
			std::cout << "Failed to load OpenAL!" << std::endl << alError << std::endl;
		}

		// Creates a new sound model.
		alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	}

	audio::~audio()
	{
	}

	void audio::update()
	{
		if (camera::get()->getCamera() != NULL)
		{
			vector3 *cameraPosition = camera::get()->getCamera()->getPosition();
			alListener3f(AL_POSITION, cameraPosition->m_x, cameraPosition->m_y, cameraPosition->m_z);
		}
	}
}
