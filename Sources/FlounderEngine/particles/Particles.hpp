#pragma once

#include <vector>
#include <map>
#include "../Engine/Engine.hpp"
#include "Particle.hpp"
#include "ParticleSystem.hpp"

namespace Flounder
{
	/// <summary>
	/// A manager that manages particles.
	/// </summary>
	class Particles :
		public IModule
	{
	private:
		static const float MAX_ELAPSED_TIME;

		std::vector<ParticleSystem*> *m_particleSystems;
		std::map<ParticleType*, std::vector<Particle*>*> *m_particles;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Particles *Get()
		{
			return static_cast<Particles*>(Engine::Get()->GetModule("particles"));
		}

		/// <summary>
		/// Creates a new particles module.
		/// </summary>
		Particles();

		/// <summary>
		/// Deconstructor for the particles module.
		/// </summary>
		~Particles();

		void Update() override;

		/// <summary>
		/// Clears all particles from the scene.
		/// </summary>
		void Clear();

		/// <summary>
		/// Adds a particle system to the recalculateRay loop.
		/// </summary>
		/// <param name="system"> The new system to add. </param>
		void AddSystem(ParticleSystem *system);

		/// <summary>
		/// Removes a particle system from the recalculateRay loop.
		/// </summary>
		/// <param name="system"> The system to remove. </param>
		void RemoveSystem(ParticleSystem *system);

		/// <summary>
		/// Gets a list of all particles.
		/// </summary>
		/// <returns> All particles. </returns>
		std::map<ParticleType*, std::vector<Particle*>*> *GetParticles() const { return m_particles; }
	};
}
