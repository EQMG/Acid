#pragma once

#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "ParticleType.hpp"

namespace acid {
/**
 * @brief A instance of a particle type.
 */
class ACID_EXPORT Particle {
	friend class ParticleType;
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
	Particle(std::shared_ptr<ParticleType> particleType, const Vector3f &position, const Vector3f &velocity, float lifeLength, float stageCycles,
		float rotation, float scale, float gravityEffect);

	/**
	 * Updates the particle.
	 */
	void Update();

	bool operator<(const Particle &rhs) const;

	bool IsAlive() const { return transparency > 0.0f; }
	const std::shared_ptr<ParticleType> &GetParticleType() const { return particleType; }
	const Vector3f &GetPosition() const { return position; }
	const Vector3f &GetVelocity() const { return velocity; }
	const Vector3f &GetChange() const { return change; }
	float GetLifeLength() const { return lifeLength; }
	float GetRotation() const { return rotation; }
	float GetScale() const { return scale; }
	float GetGravityEffect() const { return gravityEffect; }
	float GetElapsedTime() const { return elapsedTime; }
	float GetTransparency() const { return transparency; }
	float DistanceToCamera() const { return distanceToCamera; }

private:
	Vector2f CalculateImageOffset(int32_t index) const;

	std::shared_ptr<ParticleType> particleType;

	Vector3f position;
	Vector3f velocity;
	Vector3f change;

	Vector2f imageOffset1, imageOffset2;

	float lifeLength;
	float stageCycles;
	float rotation;
	float scale;
	float gravityEffect;

	float elapsedTime = 0.0f;
	float transparency = 1.0f;
	float imageBlendFactor = 0.0f;
	float distanceToCamera = 0.0f;
};
}
