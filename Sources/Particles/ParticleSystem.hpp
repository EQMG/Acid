#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Timer.hpp"
#include "Scenes/Component.hpp"
#include "Emitters/Emitter.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"

namespace acid
{
/**
 * @brief A system of particles.
 */
class ACID_EXPORT ParticleSystem :
	public Component
{
public:
	/**
	 * Creates a new particle system.
	 * @param types The types of particles to spawn.
	 * @param pps Particles per second.
	 * @param averageSpeed Particle average speed.
	 * @param gravityEffect How much gravity will effect the particles.
	 */
	explicit ParticleSystem(std::vector<std::shared_ptr<ParticleType>> types = {}, const float &pps = 5.0f, const float &averageSpeed = 0.2f, const float &gravityEffect = 1.0f);

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	void AddParticleType(const std::shared_ptr<ParticleType> &type);

	bool RemoveParticleType(const std::shared_ptr<ParticleType> &type);

	const float &GetPps() const { return m_pps; }

	void SetPps(const float &pps);

	const float &GetAverageSpeed() const { return m_averageSpeed; }

	void SetAverageSpeed(const float &averageSpeed) { m_averageSpeed = averageSpeed; }

	const float &GetGravityEffect() const { return m_gravityEffect; }

	void SetGravityEffect(const float &gravityEffect) { m_gravityEffect = gravityEffect; }

	const bool &IsRandomRotation() const { return m_randomRotation; }

	void SetRandomRotation(const bool &randomRotation) { m_randomRotation = randomRotation; }

	const Vector3 &GetDirection() const { return m_direction; }

	void SetDirection(const Vector3 &direction, const float &deviation);

	const float &GetSpeedDeviation() const { return m_speedDeviation; }

	void SetSpeedDeviation(const float &speedDeviation) { m_speedDeviation = speedDeviation; }

	const float &GetLifeDeviation() const { return m_lifeDeviation; }

	void SetLifeDeviation(const float &lifeDeviation) { m_lifeDeviation = lifeDeviation; }

	const float &GetStageDeviation() const { return m_stageDeviation; }

	void SetStageDeviation(const float &stageDeviation) { m_stageDeviation = stageDeviation; }

	const float &GetScaleDeviation() const { return m_scaleDeviation; }

	void SetScaleDeviation(const float &scaleDeviation) { m_scaleDeviation = scaleDeviation; }

private:
	Particle EmitParticle(const Emitter &emitter);

	float GenerateValue(const float &average, const float &errorPercent) const;

	float GenerateRotation() const;

	Vector3 GenerateRandomUnitVector() const;

	std::vector<std::shared_ptr<ParticleType>> m_types;

	float m_pps;
	float m_averageSpeed;
	float m_gravityEffect;
	bool m_randomRotation;

	Vector3 m_direction;
	float m_directionDeviation;
	float m_speedDeviation;
	float m_lifeDeviation;
	float m_stageDeviation;
	float m_scaleDeviation;

	Timer m_emitTimer;
};
}
