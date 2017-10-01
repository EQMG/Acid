#pragma once

#include <vector>

#include "../maths/Vector3.hpp"

#include "spawns/ISpawnParticle.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"

namespace Flounder
{
	/// <summary>
	/// A system of particles that are to be spawned.
	/// </summary>
	class ParticleSystem
	{
	private:
		std::vector<ParticleType*> *m_types;
		ISpawnParticle *m_spawn;
		float m_pps;
		float m_averageSpeed;
		float m_gravityEffect;
		bool m_randomRotation;

		Vector3 *m_systemCentre;
		Vector3 *m_velocityCentre;

		Vector3 *m_direction;
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
		/// <param name="speed"> The particle speed. </param>
		/// <param name="gravityEffect"> How much gravity will effect the particle. </param>
		ParticleSystem(std::vector<ParticleType*> *types, ISpawnParticle *spawn, const float &pps, const float &speed, const float &gravityEffect);

		/// <summary>
		/// Deconstructor for the particle system.
		/// </summary>
		~ParticleSystem();

		Particle *GenerateParticles();
	private:
		Particle *EmitParticle();

		float GenerateValue(const float &average, const float &errorMargin) const;

		float GenerateRotation() const;

		Vector3 *GenerateRandomUnitVector() const;
	public:
		void AddParticleType(ParticleType *type) const;

		void RemoveParticleType(ParticleType *type) const;

		ISpawnParticle *GetSpawn() const { return m_spawn; }

		void SetSpawn(ISpawnParticle *spawn);

		float GetPps() const { return m_pps; }

		void SetPps(const float &pps) { m_pps = pps; }

		float GetAverageSpeed() const { return m_averageSpeed; }

		void SetAverageSpeed(const float &averageSpeed) { m_averageSpeed = averageSpeed; }

		float GetGravityEffect() const { return m_gravityEffect; }

		void SetGravityEffect(const float &gravityEffect) { m_gravityEffect = gravityEffect; }

		bool GetRandomRotation() const { return m_randomRotation; }

		void SetRandomRotation(const bool &randomRotation) { m_randomRotation = randomRotation; }

		Vector3 *GetSystemCentre() const { return m_systemCentre; }

		void SetSystemCentre(const Vector3 &systemCentre) const { m_systemCentre->Set(systemCentre); }

		Vector3 *GetVelocityCentre() const { return m_velocityCentre; }

		void SetVelocityCentre(const Vector3 &velocityCentre) const { m_velocityCentre->Set(velocityCentre); }

		Vector3 *GetDirection() const { return m_direction; }

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
