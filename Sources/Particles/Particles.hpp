#pragma once

#include <map>
#include <vector>
#include "Engine/Engine.hpp"
#include "Particle.hpp"

namespace acid
{
	/// <summary>
	/// A manager that manages particles.
	/// </summary>
	class ACID_EXPORT Particles :
		public IModule
	{
	private:
		static const float MAX_ELAPSED_TIME;

		std::map<std::shared_ptr<ParticleType>, std::vector<Particle>> m_particles;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Particles *Get() { return Engine::Get()->GetModule<Particles>(); }

		Particles();

		void Update() override;

		void AddParticle(const Particle &particle);

		/// <summary>
		/// Clears all particles from the scene.
		/// </summary>
		void Clear();

		/// <summary>
		/// Gets a list of all particles.
		/// </summary>
		/// <returns> All particles. </returns>
		std::map<std::shared_ptr<ParticleType>, std::vector<Particle>> GetParticles() const { return m_particles; }
	};
}
