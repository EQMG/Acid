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
class ACID_EXPORT ParticleSystem : public Component::Registrar<ParticleSystem>, NonCopyable {
	inline static const bool Registered = Register("particleSystem");
public:
	/**
	 * Creates a new particle system.
	 * @param types The types of particles to spawn.
	 * @param emitters The emitter shapes that are used to define particle spawn volumes.
	 * @param pps Particles per second.
	 * @param averageSpeed Particle average speed.
	 * @param gravityEffect How much gravity will effect the particles.
	 */
	explicit ParticleSystem(std::vector<std::shared_ptr<ParticleType>> types = {}, std::vector<std::unique_ptr<Emitter>> &&emitters = {}, 
		float pps = 5.0f, float averageSpeed = 0.2f, float gravityEffect = 1.0f);

	void Start() override;
	void Update() override;

	void AddParticleType(const std::shared_ptr<ParticleType> &type);
	bool RemoveParticleType(const std::shared_ptr<ParticleType> &type);

	void AddEmitter(std::unique_ptr<Emitter> &&emitter);

	Vector3f RandomUnitVectorWithinCone(const Vector3f &coneDirection, float angle) const;

	float GetPps() const { return pps; }
	void SetPps(float pps);

	float GetAverageSpeed() const { return averageSpeed; }
	void SetAverageSpeed(float averageSpeed) { this->averageSpeed = averageSpeed; }

	float GetGravityEffect() const { return gravityEffect; }
	void SetGravityEffect(float gravityEffect) { this->gravityEffect = gravityEffect; }

	bool IsRandomRotation() const { return randomRotation; }
	void SetRandomRotation(bool randomRotation) { this->randomRotation = randomRotation; }

	const Vector3f &GetDirection() const { return direction; }
	void SetDirection(const Vector3f &direction, float deviation);

	float GetSpeedDeviation() const { return speedDeviation; }
	void SetSpeedDeviation(float speedDeviation) { this->speedDeviation = speedDeviation; }

	float GetLifeDeviation() const { return lifeDeviation; }
	void SetLifeDeviation(float lifeDeviation) { this->lifeDeviation = lifeDeviation; }

	float GetStageDeviation() const { return stageDeviation; }
	void SetStageDeviation(float stageDeviation) { this->stageDeviation = stageDeviation; }

	float GetScaleDeviation() const { return scaleDeviation; }
	void SetScaleDeviation(float scaleDeviation) { this->scaleDeviation = scaleDeviation; }

	friend const Node &operator>>(const Node &node, ParticleSystem &particleSystem);
	friend Node &operator<<(Node &node, const ParticleSystem &particleSystem);

private:
	Particle EmitParticle(const Emitter *emitter);
	static float GenerateValue(float average, float errorPercent);
	float GenerateRotation() const;
	Vector3f GenerateRandomUnitVector() const;

	std::vector<std::shared_ptr<ParticleType>> types;
	std::vector<std::unique_ptr<Emitter>> emitters;

	float pps;
	float averageSpeed;
	float gravityEffect;
	bool randomRotation;

	Vector3f direction;
	float directionDeviation = 0.0f;
	float speedDeviation = 0.0f;
	float lifeDeviation = 0.0f;
	float stageDeviation = 0.0f;
	float scaleDeviation = 0.0f;

	ElapsedTime elapsedEmit;
};
}
