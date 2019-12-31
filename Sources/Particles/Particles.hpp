#pragma once

#include "Engine/Engine.hpp"
#include "Particle.hpp"

namespace acid {
/**
 * @brief A manager that manages particles.
 */
class ACID_EXPORT Particles : public Module::Registrar<Particles, Module::Stage::Normal> {
public:
	using ParticlesContainer = std::map<std::shared_ptr<ParticleType>, std::vector<Particle>>;

	Particles();

	void Update() override;

	void AddParticle(Particle &&particle);
	//void RemoveParticle(const Particle &particle);

	/**
	 * Clears all particles from the scene.
	 */
	void Clear();

	/**
	 * Gets a list of all particles.
	 * @return All particles.
	 */
	const ParticlesContainer &GetParticles() const { return m_particles; }

private:
	ParticlesContainer m_particles;
};
}
