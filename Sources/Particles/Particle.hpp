﻿#pragma once

#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "ParticleType.hpp"

namespace acid
{
/**
 * @brief A instance of a particle type.
 */
class ACID_EXPORT Particle
{
public:
	/**
	 * Creates a new particle object.
	 * @param particleType The particle type to create from.
	 * @param position The particles initial position.
	 * @param velocity The particles initial velocity.
	 * @param lifeLength The particles life length.
	 * @param stageCycles The amount of times stages will be shown.
	 * @param rotation The particles rotation.
	 * @param scale The particles scale.
	 * @param gravityEffect The particles gravity effect.
	 */
	Particle(std::shared_ptr<ParticleType> particleType, const Vector3f &position, const Vector3f &velocity, const float &lifeLength, const float &stageCycles,
		const float &rotation, const float &scale, const float &gravityEffect);

	/**
	 * Updates the particle.
	 */
	void Update();

	bool IsAlive() const { return m_transparency > 0.0f; }

	const std::shared_ptr<ParticleType> &GetParticleType() const { return m_particleType; }

	const Vector3f &GetPosition() const { return m_position; }

	const Vector3f &GetVelocity() const { return m_velocity; }

	const Vector3f &GetChange() const { return m_change; }

	const float &GetLifeLength() const { return m_lifeLength; }

	const float &GetRotation() const { return m_rotation; }

	const float &GetScale() const { return m_scale; }

	const float &GetGravityEffect() const { return m_gravityEffect; }

	const float &GetElapsedTime() const { return m_elapsedTime; }

	const float &GetTransparency() const { return m_transparency; }

	const float &DistanceToCamera() const { return m_distanceToCamera; }

	bool operator<(const Particle &other) const;

private:
	friend class ParticleType;

	Vector2f CalculateImageOffset(const int32_t &index) const;

	std::shared_ptr<ParticleType> m_particleType;

	Vector3f m_position;
	Vector3f m_velocity;
	Vector3f m_change;

	Vector2f m_imageOffset1;
	Vector2f m_imageOffset2;

	float m_lifeLength;
	float m_stageCycles;
	float m_rotation;
	float m_scale;
	float m_gravityEffect;

	float m_elapsedTime;
	float m_transparency;
	float m_imageBlendFactor;
	float m_distanceToCamera;
};
}
