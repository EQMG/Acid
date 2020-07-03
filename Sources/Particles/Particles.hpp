#pragma once

#include "Scenes/System.hpp"
#include "Particle.hpp"

namespace acid {
/**
 * @brief A manager that manages particles.
 */
class ACID_EXPORT Particles : public System {
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
	const ParticlesContainer &GetParticles() const { return particles; }

private:
	ParticlesContainer particles;
};
}
