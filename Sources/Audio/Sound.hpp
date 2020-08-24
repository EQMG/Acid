#pragma once

#include "Maths/Vector3.hpp"
#include "Scenes/Component.hpp"
#include "SoundBuffer.hpp"
#include "Audio.hpp"

namespace acid {
/**
 * @brief Class that represents a playable sound.
 */
class ACID_EXPORT Sound : public Component::Registrar<Sound> {
	static const bool Registered;
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

	friend const Node &operator>>(const Node &node, Sound &sound);
	friend Node &operator<<(Node &node, const Sound &sound);

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
}
