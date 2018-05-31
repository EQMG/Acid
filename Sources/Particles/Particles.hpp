#pragma once

#include <vector>
#include <map>
#include "Engine/Engine.hpp"
#include "Particle.hpp"

namespace fl
{
	/// <summary>
	/// A manager that manages particles.
	/// </summary>
	class FL_EXPORT Particles :
		public IModule
	{
	private:
		static const float MAX_ELAPSED_TIME;

		std::map<std::shared_ptr<ParticleType>, std::vector<Particle *>> m_particles;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Particles *Get()
		{
			return reinterpret_cast<Particles *>(Engine::Get()->GetModule("particles"));
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

		void AddParticle(Particle *created);

		/// <summary>
		/// Clears all particles from the scene.
		/// </summary>
		void Clear();

		/// <summary>
		/// Gets a list of all particles.
		/// </summary>
		/// <returns> All particles. </returns>
		std::map<std::shared_ptr<ParticleType>, std::vector<Particle *>> GetParticles() const { return m_particles; }
	};
}
