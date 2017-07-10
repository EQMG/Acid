#include "audio.h"

namespace flounder {
	audio::audio()
		: imodule()
	{
	}

	audio::~audio()
	{
	}

	void audio::init()
	{
		// Creates the OpenAL contexts.
		m_device = alcOpenDevice(NULL);

		m_context = alcCreateContext(m_device, NULL);
		ALCboolean made = alcMakeContextCurrent(m_context);

		// Checks for errors.
		ALenum alError = alGetError();

		if (alError != GL_NO_ERROR) {
			//	logger::get()->error("OpenAL Error: " + alError);
		}

		// Creates a new sound model.
		alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	}

	void audio::update()
	{
	//	vector3 cameraPosition = FlounderSound.get().getCameraPosition();

	//	if (cameraPosition != NULL) {
	//		alListener3f(AL_POSITION, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	//	}
	}
}
