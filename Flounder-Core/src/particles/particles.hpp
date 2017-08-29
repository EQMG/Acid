#pragma once

#include <vector>
#include <map>
#include <algorithm>

#include "../framework/framework.hpp"
#include "../uis/uis.hpp"
#include "../space/structurebasic.hpp"

#include "particle.hpp"
#include "particlesystem.hpp"

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
		std::map<particletype*, std::vector<particle*>*> *m_particles;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline particles *get()
		{
			return static_cast<particles*>(framework::get()->getInstance("particles"));
		}

		/// <summary>
		/// Creates a new particles module.
		/// </summary>
		particles();

		/// <summary>
		/// Deconstructor for the particles module.
		/// </summary>
		~particles();

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
		inline std::map<particletype*, std::vector<particle*>*> *getParticles() const { return m_particles; }
	};
}
