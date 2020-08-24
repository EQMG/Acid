#include "Sound.hpp"

#ifdef ACID_BUILD_MACOS
#include <OpenAL/al.h>
#else
#include <al.h>
#endif
#include "Maths/Transform.hpp"
#include "Scenes/Entity.hpp"

namespace acid {
const bool Sound::Registered = Register("sound");

Sound::Sound(const std::string &filename, const Audio::Type &type, bool begin, bool loop, float gain, float pitch) :
	buffer(SoundBuffer::Create(filename)),
	type(type),
	gain(gain),
	pitch(pitch) {
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer->GetBuffer());

	Audio::CheckAl(alGetError());

	SetGain(gain);
	SetPitch(pitch);

	if (begin)
		Play(loop);

	Audio::Get()->OnGain().connect(this, [this](Audio::Type type, float volume) {
		if (this->type == type)
			SetGain(this->gain);
	});
}

Sound::~Sound() {
	alDeleteSources(1, &source);
	Audio::CheckAl(alGetError());
}

void Sound::Start() {
}

void Sound::Update() {
	if (auto transform = GetEntity()->GetComponent<Transform>()) {
		SetPosition(transform->GetPosition());
	}
}

void Sound::Play(bool loop) {
	alSourcei(source, AL_LOOPING, loop);
	alSourcePlay(source);
	Audio::CheckAl(alGetError());

	SetGain(gain);
}

void Sound::Pause() {
	if (!IsPlaying())
		return;

	alSourcePause(source);
	Audio::CheckAl(alGetError());
}

void Sound::Resume() {
	if (IsPlaying())
		return;

	alSourcePlay(source);
	Audio::CheckAl(alGetError());

	SetGain(gain);
}

void Sound::Stop() {
	if (!IsPlaying())
		return;

	alSourceStop(source);
	Audio::CheckAl(alGetError());
}

bool Sound::IsPlaying() const {
	ALenum state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void Sound::SetPosition(const Vector3f &position) {
	this->position = position;
	alSource3f(source, AL_POSITION, position.x, position.y, position.z);
	Audio::CheckAl(alGetError());
}

void Sound::SetDirection(const Vector3f &direction) {
	this->direction = direction;
	alSource3f(source, AL_DIRECTION, direction.x, direction.y, direction.z);
	Audio::CheckAl(alGetError());
}

void Sound::SetVelocity(const Vector3f &velocity) {
	this->velocity = velocity;
	alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	Audio::CheckAl(alGetError());
}

void Sound::SetGain(float gain) {
	this->gain = gain;
	alSourcef(source, AL_GAIN, gain * Audio::Get()->GetGain(type));
	Audio::CheckAl(alGetError());
}

void Sound::SetPitch(float pitch) {
	this->pitch = pitch;
	alSourcef(source, AL_PITCH, pitch);
	Audio::CheckAl(alGetError());
}

const Node &operator>>(const Node &node, Sound &sound) {
	node["buffer"].Get(sound.buffer);
	node["type"].Get(sound.type);
	node["gain"].Get(sound.gain);
	node["pitch"].Get(sound.pitch);
	return node;
}

Node &operator<<(Node &node, const Sound &sound) {
	node["buffer"].Set(sound.buffer);
	node["type"].Set(sound.type);
	node["gain"].Set(sound.gain);
	node["pitch"].Set(sound.pitch);
	return node;
}
}
