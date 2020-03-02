#include "Audio.hpp"

#include <iomanip>
#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <al.h>
#include <alc.h>
#endif

#include "Scenes/Scenes.hpp"

namespace acid {
Audio::Audio() :
	device(alcOpenDevice(nullptr)),
	context(alcCreateContext(device, nullptr)) {
	alcMakeContextCurrent(context);

#if defined(ACID_DEBUG)
	auto devices = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
	auto device = devices;
	auto next = devices + 1;

	while (device && *device != '\0' && next && *next != '\0') {
		Log::Out("Audio Device: ", device, '\n');
		auto len = std::strlen(device);
		device += len + 1;
		next += len + 2;
	}

	auto deviceName = alcGetString(this->device, ALC_DEVICE_SPECIFIER);
	Log::Out("Selected Audio Device: ", std::quoted(deviceName), '\n');
#endif
}

Audio::~Audio() {
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void Audio::Update() {
	auto camera = Scenes::Get()->GetCamera();

	if (!camera) {
		return;
	}

	// Listener gain.
	alListenerf(AL_GAIN, GetGain(Type::Master));

	// Listener position.
	auto position = camera->GetPosition();
	alListener3f(AL_POSITION, position.x, position.y, position.z);

	// Listener velocity.
	auto velocity = camera->GetPosition();
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);

	// Listener orientation.
	auto currentRay = camera->GetViewRay().GetCurrentRay();
	ALfloat orientation[6] = {currentRay.x, currentRay.y, currentRay.z, 0.0f, 1.0f, 0.0f};
	alListenerfv(AL_ORIENTATION, orientation);

	//CheckAl(alGetError());
}

std::string Audio::StringifyResultAl(int32_t result) {
	switch (result) {
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

void Audio::CheckAl(int32_t result) {
	if (result == AL_NO_ERROR) {
		return;
	}

	auto failure = StringifyResultAl(result);

	Log::Error("OpenAL error: ", failure, ", ", result, '\n');
	throw std::runtime_error("OpenAL Error: " + failure);
}

float Audio::GetGain(Type type) const {
	auto it = gains.find(type);

	if (it == gains.end()) {
		return 1.0f;
	}

	return it->second;
}

void Audio::SetGain(Type type, float volume) {
	auto it = gains.find(type);

	if (it != gains.end()) {
		it->second = volume;
		onGain(type, volume);
		return;
	}

	gains.emplace(type, volume);
	onGain(type, volume);
}
}
