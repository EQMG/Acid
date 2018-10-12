#include "Audio.hpp"

#include <cassert>
#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#if defined(ACID_BUILD_WINDOWS)
#include <windows.h>
#endif
#include "Helpers/FileSystem.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	Audio::Audio() :
		m_alDevice(nullptr),
		m_alContext(nullptr),
		m_volume(0.0f)
	{
		m_alDevice = alcOpenDevice(nullptr);
		m_alContext = alcCreateContext(m_alDevice, nullptr);
		alcMakeContextCurrent(m_alContext);
	}

	Audio::~Audio()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_alContext);
		alcCloseDevice(m_alDevice);
	}

	void Audio::Update()
	{
		auto camera = Scenes::Get()->GetScene()->GetCamera();

		if (camera == nullptr)
		{
			return;
		}

		// Listener position.
		Vector3 currentPosition = camera->GetPosition();
		alListener3f(AL_POSITION, currentPosition.m_x, currentPosition.m_y, currentPosition.m_z);

		// Listener velocity.
		Vector3 currentVelocity = camera->GetVelocity();
		alListener3f(AL_VELOCITY, currentVelocity.m_x, currentVelocity.m_y, currentVelocity.m_z);

		// Listener orientation.
		Vector3 currentRay = camera->GetViewRay().GetCurrentRay();
		ALfloat orientation[6] = {currentRay.m_x, currentRay.m_y, currentRay.m_z, 0.0f, 1.0f, 0.0f};
		alListenerfv(AL_ORIENTATION, orientation);

#ifndef ACID_BUILD_LINUX // FIXME: Fix exceptions thrown on Linux.
		CheckAl(alGetError());
#endif
	}

	std::string Audio::StringifyResultAl(const int32_t &result)
	{
		switch (result)
		{
		case AL_NO_ERROR:
			return "Success";
		case AL_INVALID_NAME:
			return "Invalid name";
		case AL_INVALID_ENUM:
			return "Invalid enum";
		case AL_INVALID_VALUE:
			return "Invalid value";
		case AL_INVALID_OPERATION:
			return "Invalid operation";
		case AL_OUT_OF_MEMORY:
			return "A memory allocation failed";
		default:
			return "ERROR: UNKNOWN AL ERROR";
		}
	}

	void Audio::CheckAl(const int32_t &result)
	{
		if (result == AL_NO_ERROR)
		{
			return;
		}

		std::string failure = StringifyResultAl(result);

		Log::Error("OpenAL error: %s, %i\n", failure.c_str(), result);
		assert(false && "OpenAL error!");
#if defined(ACID_BUILD_WINDOWS)
		MessageBox(nullptr, failure.c_str(), "OpenAL Error", 0);
#endif
	}
}
