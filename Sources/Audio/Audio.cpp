#include "Audio.hpp"

#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <al.h>
#include <alc.h>
#endif
#include "Files/FileSystem.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
Audio::Audio()
{
	m_device = alcOpenDevice(nullptr);
	m_context = alcCreateContext(m_device, nullptr);
	alcMakeContextCurrent(m_context);

#if defined(ACID_VERBOSE)
	auto devices{alcGetString(nullptr, ALC_DEVICE_SPECIFIER)};
	auto device{devices};
	auto next{devices + 1};

	while (device && *device != '\0' && next && *next != '\0')
	{
		Log::Out("Audio Device: %s\n", device);
		auto len{std::strlen(device)};
		device += len + 1;
		next += len + 2;
	}

	auto deviceName{alcGetString(m_device, ALC_DEVICE_SPECIFIER)};
	Log::Out("Selected Audio Device: '%s'\n", deviceName);
#endif
}

Audio::~Audio()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);
}

void Audio::Update()
{
	auto camera{Scenes::Get()->GetCamera()};

	if (camera == nullptr)
	{
		return;
	}

	// Listener gain.
	alListenerf(AL_GAIN, GetGain(Type::Master));

	// Listener position.
	auto position{camera->GetPosition()};
	alListener3f(AL_POSITION, position.m_x, position.m_y, position.m_z);

	// Listener velocity.
	auto velocity{camera->GetPosition()};
	alListener3f(AL_VELOCITY, velocity.m_x, velocity.m_y, velocity.m_z);

	// Listener orientation.
	auto currentRay{camera->GetViewRay().GetCurrentRay()};
	ALfloat orientation[6]{ currentRay.m_x, currentRay.m_y, currentRay.m_z, 0.0f, 1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, orientation);

	//CheckAl(alGetError());
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

	auto failure{StringifyResultAl(result)};

	Log::Error("OpenAL error: %s, %i\n", failure, result);
	Log::Popup("OpenAL Error", failure);
	throw std::runtime_error("OpenAL Error: " + result);
}

float Audio::GetGain(const Type &type) const
{
	auto it{m_gains.find(type)};

	if (it == m_gains.end())
	{
		return 1.0f;
	}

	return it->second;
}

void Audio::SetGain(const Type &type, const float &volume)
{
	auto it{m_gains.find(type)};

	if (it != m_gains.end())
	{
		it->second = volume;
		m_onGain(type, volume);
		return;
	}

	m_gains.emplace(type, volume);
	m_onGain(type, volume);
}
}
