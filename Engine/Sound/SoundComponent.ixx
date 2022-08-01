module;

#include "Maths/Vector3.hpp"
#include "Scenes/Component.hpp"

#ifdef ACID_BUILD_MACOS
#include <OpenAL/al.h>
#else
#include <al.h>
#endif
#include "Maths/Transform.hpp"
#include "Scenes/Entity.hpp"

export module acid.sound.component;
import acid.sound;
import acid.soundbuffer;

export namespace acid {
/**
 * @brief Class that represents a playable sound.
 */
class Sound : public Component::Registrar<Sound> {
	inline static const bool Registered = Register("sound");
public:
	Sound() = default;
	explicit Sound(const std::string &filename, const Audio::Type &type = Audio::Type::General, bool begin = false,
		bool loop = false, float gain = 1.0f, float pitch = 1.0f);
	~Sound();

	void Start() override;
	void Update() override;

	void Play(bool loop = false);
	void Pause();
	void Resume();
	void Stop();

	bool IsPlaying() const;

	void SetPosition(const Vector3f &position);
	void SetDirection(const Vector3f &direction);
	void SetVelocity(const Vector3f &velocity);

	const Audio::Type &GetType() const { return type; }
	void SetType(const Audio::Type &type) { this->type = type; }

	float GetGain() const { return gain; }
	void SetGain(float gain);

	float GetPitch() const { return pitch; }
	void SetPitch(float pitch);

	friend const Node &operator>>(const Node &node, Sound &sound) {
		node["buffer"].Get(sound.buffer);
		node["type"].Get(sound.type);
		node["gain"].Get(sound.gain);
		node["pitch"].Get(sound.pitch);
		return node;
	}

	friend Node &operator<<(Node &node, const Sound &sound) {
		node["buffer"].Set(sound.buffer);
		node["type"].Set(sound.type);
		node["gain"].Set(sound.gain);
		node["pitch"].Set(sound.pitch);
		return node;
	}

private:
	std::shared_ptr<SoundBuffer> buffer;
	uint32_t source = 0;

	Vector3f position;
	Vector3f direction;
	Vector3f velocity;

	Audio::Type type = Audio::Type::General;
	float gain = 1.0f;
	float pitch = 1.0f;
};

Sound::Sound(const std::string &filename, const Audio::Type &type, bool begin, bool loop, float gain, float pitch) :
	buffer(SoundBuffer::Create(filename)),
	type(type),
	gain(gain),
	pitch(pitch) {
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer->GetBuffer());

	CheckAl(alGetError());

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
	CheckAl(alGetError());
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
	CheckAl(alGetError());

	SetGain(gain);
}

void Sound::Pause() {
	if (!IsPlaying())
		return;

	alSourcePause(source);
	CheckAl(alGetError());
}

void Sound::Resume() {
	if (IsPlaying())
		return;

	alSourcePlay(source);
	CheckAl(alGetError());

	SetGain(gain);
}

void Sound::Stop() {
	if (!IsPlaying())
		return;

	alSourceStop(source);
	CheckAl(alGetError());
}

bool Sound::IsPlaying() const {
	ALenum state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void Sound::SetPosition(const Vector3f &position) {
	this->position = position;
	alSource3f(source, AL_POSITION, position.x, position.y, position.z);
	CheckAl(alGetError());
}

void Sound::SetDirection(const Vector3f &direction) {
	this->direction = direction;
	alSource3f(source, AL_DIRECTION, direction.x, direction.y, direction.z);
	CheckAl(alGetError());
}

void Sound::SetVelocity(const Vector3f &velocity) {
	this->velocity = velocity;
	alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	CheckAl(alGetError());
}

void Sound::SetGain(float gain) {
	this->gain = gain;
	alSourcef(source, AL_GAIN, gain * Audio::Get()->GetGain(type));
	CheckAl(alGetError());
}

void Sound::SetPitch(float pitch) {
	this->pitch = pitch;
	alSourcef(source, AL_PITCH, pitch);
	CheckAl(alGetError());
}

}
