#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Transform.hpp"
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
	explicit Sound(const std::string &filename, const Transform &localTransform = Transform::Identity, const Audio::Type &type = Audio::Type::General, const bool &begin = false,
		const bool &loop = false, const float &gain = 1.0f, const float &pitch = 1.0f);

	~Sound();

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	void Play(const bool &loop = false);

	void Pause();

	void Resume();

	void Stop();

	bool IsPlaying() const;

	const Transform &GetLocalTransform() const { return m_localTransform; }

	void SetLocalTransform(const Transform &localTransform) { m_localTransform = localTransform; }

	Transform GetWorldTransform() const;

	void SetPosition(const Vector3 &position);

	void SetDirection(const Vector3 &direction);

	void SetVelocity(const Vector3 &velocity);

	const Audio::Type &GetType() const { return m_type; }

	void SetType(const Audio::Type &type) { m_type = type; }

	const float &GetGain() const { return m_gain; }

	void SetGain(const float &gain);

	const float &GetPitch() const { return m_pitch; }

	void SetPitch(const float &pitch);

private:
	std::shared_ptr<SoundBuffer> m_soundBuffer;
	uint32_t m_source;

	Transform m_localTransform;
	mutable Transform m_worldTransform;
	Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_velocity;

	Audio::Type m_type;
	float m_gain;
	float m_pitch;
};
}
