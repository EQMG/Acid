#pragma once

#include <vector>

#include "../engine/Engine.hpp"
#include "../maths/Vector3.hpp"
#include "../maths/Vector4.hpp"
#include "../maths/Matrix4.hpp"

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
		std::vector<particletype*> *m_types;
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
		ParticleSystem(std::vector<particletype*> *types, ISpawnParticle *spawn, const float &pps, const float &speed, const float &gravityEffect);

		/// <summary>
		/// Deconstructor for the particle system.
		/// </summary>
		~ParticleSystem();

		Particle *GenerateParticles();
	private:
		Particle *EmitParticle();

		float GenerateValue(const float &average, const float &errorMargin);

		float GenerateRotation();

		Vector3 *GenerateRandomUnitVector();
	public:
		void AddParticleType(particletype *type);

		void RemoveParticleType(particletype *type);

		ISpawnParticle *GetSpawn() const { return m_spawn; }

		void SetSpawn(ISpawnParticle *spawn);

		float GetPps() const { return m_pps; }

		void SetPps(const float &pps) { m_pps = pps; }

		float GetAverageSpeed() const { return m_averageSpeed; }

		void setAverageSpeed(const float &averageSpeed) { m_averageSpeed = averageSpeed; }

		float getGravityEffect() const { return m_gravityEffect; }

		void setGravityEffect(const float &gravityEffect) { m_gravityEffect = gravityEffect; }

		bool getRandomRotation() const { return m_randomRotation; }

		void setRandomRotation(const bool &randomRotation) { m_randomRotation = randomRotation; }

		Vector3 *getSystemCentre() const { return m_systemCentre; }

		void setSystemCentre(const Vector3 &systemCentre) const { m_systemCentre->Set(systemCentre); }

		Vector3 *getVelocityCentre() const { return m_velocityCentre; }

		void setVelocityCentre(const Vector3 &velocityCentre) const { m_velocityCentre->Set(velocityCentre); }

		Vector3 *getDirection() const { return m_direction; }

		void setDirection(const Vector3 &direction, const float &deviation);

		float getSpeedError() const { return m_speedError; }

		void setSpeedError(const float &speedError) { m_speedError = speedError; }

		float getLifeError() const { return m_lifeError; }

		void setLifeError(const float &lifeError) { m_lifeError = lifeError; }

		float getScaleError() const { return m_scaleError; }

		void setScaleError(const float &scaleError) { m_scaleError = scaleError; }

		bool getPaused() const { return m_paused; }

		void setPaused(const bool &paused) { m_paused = paused; }
	};
}
