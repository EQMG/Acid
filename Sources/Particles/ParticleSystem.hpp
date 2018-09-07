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

		Vector3 m_systemOffset;
		Vector3 m_direction;
		float m_directionDeviation;
		float m_speedError;
		float m_lifeError;
		float m_scaleError;

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
		/// <param name="systemOffset"> The offset from the parents centre. </param>
		ParticleSystem(const std::vector<std::shared_ptr<ParticleType>> &types = std::vector<std::shared_ptr<ParticleType>>(), ISpawnParticle *spawn = nullptr, const float &pps = 5.0f, const float &averageSpeed = 0.2f, const float &gravityEffect = 1.0f, const Vector3 &systemOffset = Vector3::ZERO);

		~ParticleSystem();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

	private:
		std::optional<Particle> EmitParticle();

		float GenerateValue(const float &average, const float &errorMargin) const;

		float GenerateRotation() const;

		Vector3 GenerateRandomUnitVector() const;

	public:
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

		bool GetRandomRotation() const { return m_randomRotation; }

		void SetRandomRotation(const bool &randomRotation) { m_randomRotation = randomRotation; }

		Vector3 GetOffsetCentre() const { return m_systemOffset; }

		void SetOffsetCentre(const Vector3 &systemOffsetCentre) { m_systemOffset = systemOffsetCentre; }

		Vector3 GetDirection() const { return m_direction; }

		void SetDirection(const Vector3 &direction, const float &deviation);

		float GetSpeedError() const { return m_speedError; }

		void SetSpeedError(const float &speedError) { m_speedError = speedError; }

		float GetLifeError() const { return m_lifeError; }

		void SetLifeError(const float &lifeError) { m_lifeError = lifeError; }

		float GetScaleError() const { return m_scaleError; }

		void SetScaleError(const float &scaleError) { m_scaleError = scaleError; }

		bool GetPaused() const { return m_paused; }

		void SetPaused(const bool &paused) { m_paused = paused; }
	};
}
