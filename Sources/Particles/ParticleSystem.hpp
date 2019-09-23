#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/ElapsedTime.hpp"
#include "Scenes/Component.hpp"
#include "Emitters/Emitter.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"

namespace acid {
/**
 * @brief A system of particles.
 */
class ACID_EXPORT ParticleSystem : public Component {
public:
	/**
	 * Creates a new particle system.
	 * @param types The types of particles to spawn.
	 * @param pps Particles per second.
	 * @param averageSpeed Particle average speed.
	 * @param gravityEffect How much gravity will effect the particles.
	 */
	explicit ParticleSystem(std::vector<std::shared_ptr<ParticleType>> types = {}, float pps = 5.0f, float averageSpeed = 0.2f, float gravityEffect = 1.0f);

	void Start() override;
	void Update() override;

	void AddParticleType(const std::shared_ptr<ParticleType> &type);
	bool RemoveParticleType(const std::shared_ptr<ParticleType> &type);

	Vector3f RandomUnitVectorWithinCone(const Vector3f &coneDirection, float angle) const;

	float GetPps() const { return m_pps; }
	void SetPps(float pps);

	float GetAverageSpeed() const { return m_averageSpeed; }
	void SetAverageSpeed(float averageSpeed) { m_averageSpeed = averageSpeed; }

	float GetGravityEffect() const { return m_gravityEffect; }
	void SetGravityEffect(float gravityEffect) { m_gravityEffect = gravityEffect; }

	bool IsRandomRotation() const { return m_randomRotation; }
	void SetRandomRotation(bool randomRotation) { m_randomRotation = randomRotation; }

	const Vector3f &GetDirection() const { return m_direction; }
	void SetDirection(const Vector3f &direction, float deviation);

	float GetSpeedDeviation() const { return m_speedDeviation; }
	void SetSpeedDeviation(float speedDeviation) { m_speedDeviation = speedDeviation; }

	float GetLifeDeviation() const { return m_lifeDeviation; }
	void SetLifeDeviation(float lifeDeviation) { m_lifeDeviation = lifeDeviation; }

	float GetStageDeviation() const { return m_stageDeviation; }
	void SetStageDeviation(float stageDeviation) { m_stageDeviation = stageDeviation; }

	float GetScaleDeviation() const { return m_scaleDeviation; }
	void SetScaleDeviation(float scaleDeviation) { m_scaleDeviation = scaleDeviation; }

	friend const Node &operator>>(const Node &node, ParticleSystem &particleSystem);
	friend Node &operator<<(Node &node, const ParticleSystem &particleSystem);

private:
	Particle EmitParticle(const Emitter &emitter);
	static float GenerateValue(float average, float errorPercent);
	float GenerateRotation() const;
	Vector3f GenerateRandomUnitVector() const;

	static Registrar<ParticleSystem> registered;

	std::vector<std::shared_ptr<ParticleType>> m_types;

	float m_pps;
	float m_averageSpeed;
	float m_gravityEffect;
	bool m_randomRotation;

	Vector3f m_direction;
	float m_directionDeviation = 0.0f;
	float m_speedDeviation = 0.0f;
	float m_lifeDeviation = 0.0f;
	float m_stageDeviation = 0.0f;
	float m_scaleDeviation = 0.0f;

	ElapsedTime m_elapsedEmit;
};
}
