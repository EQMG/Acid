module;

#include <rocket.hpp>

#include "Engine/Engine.hpp"

#include <iomanip>
#ifdef ACID_BUILD_MACOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <al.h>
#include <alc.h>
#endif

#include "Scenes/Scenes.hpp"

export module acid.sound;

export namespace acid {
std::string StringifyResultAl(int32_t result) {
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

void CheckAl(int32_t result) {
	if (result == AL_NO_ERROR) return;

	auto failure = StringifyResultAl(result);

	Log::Error("OpenAL error: ", failure, ", ", result, '\n');
	throw std::runtime_error("OpenAL Error: " + failure);
}

/**
 * @brief Module used for loading, managing and playing a variety of different sound types.
 */
class Audio : public Module::Registrar<Audio> {
	inline static const bool Registered = Register(Stage::Pre);
public:
	enum class Type {
		Master, General, Effect, Music
	};

	Audio() :
		device(alcOpenDevice(nullptr)),
		context(alcCreateContext(device, nullptr)) {
		alcMakeContextCurrent(context);
#ifdef ACID_DEBUG
		auto devices = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
		auto d = devices;
		auto next = devices + 1;

		while (d && *d != '\0' && next && *next != '\0') {
			Log::Out("Audio Device: ", d, '\n');
			auto len = std::strlen(d);
			d += len + 1;
			next += len + 2;
		}

		auto deviceName = alcGetString(device, ALC_DEVICE_SPECIFIER);
		Log::Out("Selected Audio Device: ", std::quoted(deviceName), '\n');
#endif
	}

	~Audio() {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	void Update() override {
		auto scene = Scenes::Get()->GetScene();

		if (!scene) return;

		auto camera = scene->GetCamera();

		if (!camera) return;

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

	float GetGain(Type type) const {
		if (auto it = gains.find(type); it != gains.end())
			return it->second;
		return 1.0f;
	}

	void SetGain(Type type, float volume) {
		auto it = gains.find(type);

		if (it != gains.end()) {
			it->second = volume;
			onGain(type, volume);
			return;
		}

		gains.emplace(type, volume);
		onGain(type, volume);
	}

	/**
	 * Called when a gain value has been modified.
	 * @return The delegate.
	 */
	rocket::signal<void(Type, float)> &OnGain() { return onGain; }

private:
	ALCdevice *device = nullptr;
	ALCcontext *context = nullptr;

	std::map<Type, float> gains;

	rocket::signal<void(Type, float)> onGain;
};

}
