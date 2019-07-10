#pragma once

#include "Maths/Vector3.hpp"
#include "Scenes/Component.hpp"
#include "SoundBuffer.hpp"
#include "Audio.hpp"

namespace acid
{
/**
 * @brief Class that represents a playable sound.
 */
class ACID_EXPORT Sound :
	public Component
{
public:
	explicit Sound(const std::string &filename, const Audio::Type &type = Audio::Type::General, const bool &begin = false,
		const bool &loop = false, const float &gain = 1.0f, const float &pitch = 1.0f);

	~Sound();

	void Start() override;

	void Update() override;

	void Play(const bool &loop = false);

	void Pause();

	void Resume();

	void Stop();

	bool IsPlaying() const;

	void SetPosition(const Vector3f &position);

	void SetDirection(const Vector3f &direction);

	void SetVelocity(const Vector3f &velocity);

	const Audio::Type &GetType() const { return m_type; }

	void SetType(const Audio::Type &type) { m_type = type; }

	const float &GetGain() const { return m_gain; }

	void SetGain(const float &gain);

	const float &GetPitch() const { return m_pitch; }

	void SetPitch(const float &pitch);

	friend const Node &operator>>(const Node &node, Sound &sound);

	friend Node &operator<<(Node &node, const Sound &sound);

private:
	std::shared_ptr<SoundBuffer> m_buffer;
	uint32_t m_source{};

	Vector3f m_position;
	Vector3f m_direction;
	Vector3f m_velocity;

	Audio::Type m_type;
	float m_gain;
	float m_pitch;
};
}
