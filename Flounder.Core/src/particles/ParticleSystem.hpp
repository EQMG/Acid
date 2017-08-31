#pragma once

#include <vector>

#include "../engine/Engine.hpp"
#include "../maths/Vector3.hpp"
#include "../maths/Vector4.hpp"
#include "../maths/Matrix4.hpp"

#include "spawns/ispawnparticle.hpp"
#include "particletype.hpp"
#include "particle.hpp"

namespace Flounder
{
	/// <summary>
	/// A system of particles that are to be spawned.
	/// </summary>
	class particlesystem
	{
	private:
		std::vector<particletype*> *m_types;
		ispawnparticle *m_spawn;
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
		particlesystem(std::vector<particletype*> *types, ispawnparticle *spawn, const float &pps, const float &speed, const float &gravityEffect);

		/// <summary>
		/// Deconstructor for the particle system.
		/// </summary>
		~particlesystem();

		particle *generateParticles();
	private:
		particle *emitParticle();

		float generateValue(const float &average, const float &errorMargin);

		float generateRotation();

		static Vector3 *generateRandomUnitVectorWithinCone(Vector3 *coneDirection, const float &angle);

		Vector3 *generateRandomUnitVector();
	public:
		void addParticleType(particletype *type);

		void removeParticleType(particletype *type);

		ispawnparticle *getSpawn() const { return m_spawn; }

		inline void setSpawn(ispawnparticle *spawn);

		float getPps() const { return m_pps; }

		void setPps(const float &pps) { m_pps = pps; }

		float getAverageSpeed() const { return m_averageSpeed; }

		void setAverageSpeed(const float &averageSpeed) { m_averageSpeed = averageSpeed; }

		float getGravityEffect() const { return m_gravityEffect; }

		void setGravityEffect(const float &gravityEffect) { m_gravityEffect = gravityEffect; }

		bool getRandomRotation() const { return m_randomRotation; }

		void setRandomRotation(const bool &randomRotation) { m_randomRotation = randomRotation; }

		Vector3 *getSystemCentre() const { return m_systemCentre; }

		void setSystemCentre(const Vector3 &systemCentre) const { m_systemCentre->set(systemCentre); }

		Vector3 *getVelocityCentre() const { return m_velocityCentre; }

		void setVelocityCentre(const Vector3 &velocityCentre) const { m_velocityCentre->set(velocityCentre); }

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
