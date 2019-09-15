#include "Sound.hpp"

#if defined(ACID_BUILD_MACOS)
#include <OpenAL/al.h>
#else
#include <al.h>
#endif
#include "Maths/Transform.hpp"
#include "Scenes/Entity.hpp"

namespace acid {
Sound::Sound(const std::string &filename, const Audio::Type &type, bool begin, bool loop, float gain, float pitch) :
	m_buffer(SoundBuffer::Create(filename)),
	m_type(type),
	m_gain(gain),
	m_pitch(pitch) {
	alGenSources(1, &m_source);
	alSourcei(m_source, AL_BUFFER, m_buffer->GetBuffer());

	Audio::CheckAl(alGetError());

	SetGain(gain);
	SetPitch(pitch);

	if (begin) {
		Play(loop);
	}

	Audio::Get()->OnGain().Add([this](Audio::Type type, float volume) {
		if (type == m_type) {
			SetGain(m_gain);
		}
	}, this);
}

Sound::~Sound() {
	alDeleteSources(1, &m_source);
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
	alSourcei(m_source, AL_LOOPING, loop);
	alSourcePlay(m_source);
	Audio::CheckAl(alGetError());

	SetGain(m_gain);
}

void Sound::Pause() {
	if (!IsPlaying()) {
		return;
	}

	alSourcePause(m_source);
	Audio::CheckAl(alGetError());
}

void Sound::Resume() {
	if (IsPlaying()) {
		return;
	}

	alSourcePlay(m_source);
	Audio::CheckAl(alGetError());

	SetGain(m_gain);
}

void Sound::Stop() {
	if (!IsPlaying()) {
		return;
	}

	alSourceStop(m_source);
	Audio::CheckAl(alGetError());
}

bool Sound::IsPlaying() const {
	ALenum state;
	alGetSourcei(m_source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void Sound::SetPosition(const Vector3f &position) {
	m_position = position;
	alSource3f(m_source, AL_POSITION, m_position.m_x, m_position.m_y, m_position.m_z);
	Audio::CheckAl(alGetError());
}

void Sound::SetDirection(const Vector3f &direction) {
	m_direction = direction;
	alSource3f(m_source, AL_DIRECTION, m_direction.m_x, m_direction.m_y, m_direction.m_z);
	Audio::CheckAl(alGetError());
}

void Sound::SetVelocity(const Vector3f &velocity) {
	m_velocity = velocity;
	alSource3f(m_source, AL_VELOCITY, m_velocity.m_x, m_velocity.m_y, m_velocity.m_z);
	Audio::CheckAl(alGetError());
}

void Sound::SetGain(float gain) {
	m_gain = gain;
	alSourcef(m_source, AL_GAIN, m_gain * Audio::Get()->GetGain(m_type));
	Audio::CheckAl(alGetError());
}

void Sound::SetPitch(float pitch) {
	m_pitch = pitch;
	alSourcef(m_source, AL_PITCH, m_pitch);
	Audio::CheckAl(alGetError());
}

const Node &operator>>(const Node &node, Sound &sound) {
	node["buffer"].Get(sound.m_buffer);
	node["type"].Get(sound.m_type);
	node["gain"].Get(sound.m_gain);
	node["pitch"].Get(sound.m_pitch);
	return node;
}

Node &operator<<(Node &node, const Sound &sound) {
	node["buffer"].Set(sound.m_buffer);
	node["type"].Set(sound.m_type);
	node["gain"].Set(sound.m_gain);
	node["pitch"].Set(sound.m_pitch);
	return node;
}
}
