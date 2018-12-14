#pragma once

#include <vector>
#include "Maths/Vector3.hpp"
#include "Maths/Timer.hpp"
#include "Scenes/Entity.hpp"
#include "Scenes/Component.hpp"
#include "Emitters/ParticleEmitter.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"

namespace acid
{
	/// <summary>
	/// A system of particles that are to be spawned.
	/// </summary>
	class ACID_EXPORT ParticleSystem :
		public Component
	{
	private:
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
	public:
		/// <summary>
		/// Creates a new particle system.
		/// </summary>
		/// <param name="types"> The types of particles to spawn. </param>
		/// <param name="localTransform"> The local transform from the parents space. </param>
		/// <param name="pps"> Particles per second. </param>
		/// <param name="averageSpeed"> Particle average speed. </param>
		/// <param name="gravityEffect"> How much gravity will effect the particles. </param>
		explicit ParticleSystem(const std::vector<std::shared_ptr<ParticleType>> &types = {}, const float &pps = 5.0f, const float &averageSpeed = 0.2f, const float &gravityEffect = 1.0f);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void AddParticleType(const std::shared_ptr<ParticleType> &type);

		bool RemoveParticleType(const std::shared_ptr<ParticleType> &type);

		float GetPps() const { return m_pps; }

		void SetPps(const float &pps);

		float GetAverageSpeed() const { return m_averageSpeed; }

		void SetAverageSpeed(const float &averageSpeed) { m_averageSpeed = averageSpeed; }

		float GetGravityEffect() const { return m_gravityEffect; }

		void SetGravityEffect(const float &gravityEffect) { m_gravityEffect = gravityEffect; }

		bool IsRandomRotation() const { return m_randomRotation; }

		void SetRandomRotation(const bool &randomRotation) { m_randomRotation = randomRotation; }

		Vector3 GetDirection() const { return m_direction; }

		void SetDirection(const Vector3 &direction, const float &deviation);

		float GetSpeedDeviation() const { return m_speedDeviation; }

		void SetSpeedDeviation(const float &speedDeviation) { m_speedDeviation = speedDeviation; }

		float GetLifeDeviation() const { return m_lifeDeviation; }

		void SetLifeDeviation(const float &lifeDeviation) { m_lifeDeviation = lifeDeviation; }

		float GetStageDeviation() const { return m_stageDeviation; }

		void SetStageDeviation(const float &stageDeviation) { m_stageDeviation = stageDeviation; }

		float GetScaleDeviation() const { return m_scaleDeviation; }

		void SetScaleDeviation(const float &scaleDeviation) { m_scaleDeviation = scaleDeviation; }
	private:
		Particle EmitParticle(const ParticleEmitter &emitter);

		float GenerateValue(const float &average, const float &errorPercent) const;

		float GenerateRotation() const;

		Vector3 GenerateRandomUnitVector() const;
	};
}
