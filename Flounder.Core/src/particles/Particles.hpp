#pragma once

#include <vector>
#include <map>
#include <algorithm>

#include "../engine/Engine.hpp"
#include "../uis/uis.hpp"
#include "../space/StructureBasic.hpp"

#include "particle.hpp"
#include "particlesystem.hpp"

namespace Flounder
{
	/// <summary>
	/// A manager that manages particles.
	/// </summary>
	class particles :
		public IModule
	{
	private:
		static const float MAX_ELAPSED_TIME;

		std::vector<particlesystem*> *m_particleSystems;
		std::map<particletype*, std::vector<particle*>*> *m_particles;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static particles *get()
		{
			return static_cast<particles*>(Engine::Get()->GetModule("particles"));
		}

		/// <summary>
		/// Creates a new particles module.
		/// </summary>
		particles();

		/// <summary>
		/// Deconstructor for the particles module.
		/// </summary>
		~particles();

		void Update() override;

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
		std::map<particletype*, std::vector<particle*>*> *getParticles() const { return m_particles; }
	};
}
