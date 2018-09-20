#pragma once

#include <vector>
#include <optional>
#include "Maths/Vector3.hpp"
#include "Objects/GameObject.hpp"
#include "Objects/IComponent.hpp"
#include "Spawns/ISpawnParticle.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"

namespace acid
{
	/// <summary>
	/// A system of particles that are to be spawned.
	/// </summary>
	class ACID_EXPORT ParticleSystem :
		public IComponent
	{
	private:
		std::vector<std::shared_ptr<ParticleType>> m_types;
		std::unique_ptr<ISpawnParticle> m_spawn;

		float m_pps;
		float m_averageSpeed;
		float m_gravityEffect;
		bool m_randomRotation;

		Vector3 m_lastPosition;

		Vector3 m_localOffset;
		Vector3 m_direction;
		float m_directionDeviation;
		float m_speedDeviation;
		float m_lifeDeviation;
		float m_stageDeviation;
		float m_scaleDeviation;

		float m_timePassed;
		bool m_paused;
	public:
		/// <summary>
		/// Creates a new particle system.
		/// </summary>
		/// <param name="types"> The types of particles to spawn. </param>
		/// <param name="spawn"> The particle spawn types. </param>
		/// <param name="pps"> Particles per second. </param>
		/// <param name="averageSpeed"> Particle average speed. </param>
		/// <param name="gravityEffect"> How much gravity will effect the particles. </param>
		/// <param name="localOffset"> The offset from the game objects centre. </param>
		explicit ParticleSystem(const std::vector<std::shared_ptr<ParticleType>> &types = {}, ISpawnParticle *spawn = nullptr, const float &pps = 5.0f, const float &averageSpeed = 0.2f, const float &gravityEffect = 1.0f, const Vector3 &localOffset = Vector3::ZERO);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void AddParticleType(const std::shared_ptr<ParticleType> &type);

		bool RemoveParticleType(const std::shared_ptr<ParticleType> &type);

		ISpawnParticle *GetSpawn() const { return m_spawn.get(); }

		void SetSpawn(ISpawnParticle *spawn) { m_spawn.reset(spawn); }

		void TrySetSpawn(const Metadata &spawnNode);

		float GetPps() const { return m_pps; }

		void SetPps(const float &pps) { m_pps = pps; }

		float GetAverageSpeed() const { return m_averageSpeed; }

		void SetAverageSpeed(const float &averageSpeed) { m_averageSpeed = averageSpeed; }

		float GetGravityEffect() const { return m_gravityEffect; }

		void SetGravityEffect(const float &gravityEffect) { m_gravityEffect = gravityEffect; }

		bool IsRandomRotation() const { return m_randomRotation; }

		void SetRandomRotation(const bool &randomRotation) { m_randomRotation = randomRotation; }

		Vector3 GetLocalOffset() const { return m_localOffset; }

		void SetLocalOffset(const Vector3 &localOffset) { m_localOffset = localOffset; }

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

		bool IsPaused() const { return m_paused; }

		void SetPaused(const bool &paused) { m_paused = paused; }
	private:
		std::optional<Particle> EmitParticle();

		float GenerateValue(const float &average, const float &errorPercent) const;

		float GenerateRotation() const;

		Vector3 GenerateRandomUnitVector() const;
	};
}
