#pragma once

#include <vector>

#include "../framework/framework.hpp"
#include "../maths/vector3.hpp"
#include "../maths/vector4.hpp"
#include "../maths/matrix4x4.hpp"

#include "spawns/ispawnparticle.hpp"
#include "particletype.hpp"
#include "particle.hpp"

namespace flounder
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

		vector3 *m_systemCentre;
		vector3 *m_velocityCentre;

		vector3 *m_direction;
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

		static vector3 *generateRandomUnitVectorWithinCone(vector3 *coneDirection, const float &angle);

		vector3 *generateRandomUnitVector();
	public:
		void addParticleType(particletype *type);

		void removeParticleType(particletype *type);

		inline ispawnparticle *getSpawn() const { return m_spawn; }

		inline void setSpawn(ispawnparticle *spawn);

		inline float getPps() const { return m_pps; }

		inline void setPps(const float &pps) { m_pps = pps; }

		inline float getAverageSpeed() const { return m_averageSpeed; }

		inline void setAverageSpeed(const float &averageSpeed) { m_averageSpeed = averageSpeed; }

		inline float getGravityEffect() const { return m_gravityEffect; }

		inline void setGravityEffect(const float &gravityEffect) { m_gravityEffect = gravityEffect; }

		inline bool getRandomRotation() const { return m_randomRotation; }

		inline void setRandomRotation(const bool &randomRotation) { m_randomRotation = randomRotation; }

		inline vector3 *getSystemCentre() const { return m_systemCentre; }

		inline void setSystemCentre(const vector3 &systemCentre) const { m_systemCentre->set(systemCentre); }

		inline vector3 *getVelocityCentre() const { return m_velocityCentre; }

		inline void setVelocityCentre(const vector3 &velocityCentre) const { m_velocityCentre->set(velocityCentre); }

		inline vector3 *getDirection() const { return m_direction; }

		void setDirection(const vector3 &direction, const float &deviation);

		inline float getSpeedError() const { return m_speedError; }

		inline void setSpeedError(const float &speedError) { m_speedError = speedError; }

		inline float getLifeError() const { return m_lifeError; }

		inline void setLifeError(const float &lifeError) { m_lifeError = lifeError; }

		inline float getScaleError() const { return m_scaleError; }

		inline void setScaleError(const float &scaleError) { m_scaleError = scaleError; }

		inline bool getPaused() const { return m_paused; }

		inline void setPaused(const bool &paused) { m_paused = paused; }
	};
}
