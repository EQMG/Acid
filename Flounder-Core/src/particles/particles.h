#pragma once

#include <vector>
#include <algorithm>

#include "../framework/framework.h"
#include "../space/structurebasic.h"

#include "particle.h"
#include "particlesystem.h"

namespace flounder
{
	/// <summary>
	/// A manager that manages particles.
	/// </summary>
	class particles :
		public imodule
	{
	private:
		static const float MAX_ELAPSED_TIME;

		std::vector<particlesystem*> *m_particleSystems;
		std::vector<structurebasic<particle*>*> *m_particles;
		std::vector<particle*> *m_deadParticles;

	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static particles *get()
		{
			return static_cast<particles*>(framework::get()->getInstance("particles"));
		}

		particles();

		~particles();

		void init() override;

		void update() override;

		/// <summary>
		/// Clears all particles from the scene.
		/// </summary>
		void clear();

		/// <summary>
		/// Adds a particle system to the recalculateRay loop.
		/// </summary>
		/// <param name="system"> The new system to add. </param>
		void addSystem(particlesystem *system);

		/// <summary>
		/// Removes a particle system from the recalculateRay loop.
		/// </summary>
		/// <param name="system"> The system to remove. </param>
		void removeSystem(particlesystem *system);

		/// <summary>
		/// Gets a list of all particles.
		/// </summary>
		/// <returns> All particles. </returns>
		inline std::vector<structurebasic<particle*>*> *getParticles() { return m_particles; }

		/// <summary>
		/// Adds a particle to the recalculateRay loop.
		/// </summary>
		/// <param name="particleType"> The particle template to build from. </param>
		/// <param name="position"> The particles initial position. </param>
		/// <param name="velocity"> The particles initial velocity. </param>
		/// <param name="lifeLength"> The particles life length. </param>
		/// <param name="rotation"> The particles rotation. </param>
		/// <param name="scale"> The particles scale. </param>
		/// <param name="gravityEffect"> The particles gravity effect. </param>
		virtual void addParticle(particletype *particleType, const vector3 &position, const vector3 &velocity, const float &lifeLength, const float &rotation, const float &scale, const float &gravityEffect);
	};
}
