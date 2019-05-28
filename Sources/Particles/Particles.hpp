#pragma once

#include <map>
#include <vector>
#include "Engine/Engine.hpp"
#include "Particle.hpp"

namespace acid
{
/**
 * @brief A manager that manages particles.
 */
class Particles :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Particles *Get() { return Engine::Get()->GetModule<Particles>(); }

	Particles();

	void Update() override;

	void AddParticle(const Particle &particle);

	//void RemoveParticle(const Particle &particle);

	/**
	 * Clears all particles from the scene.
	 */
	void Clear();

	/**
	 * Gets a list of all particles.
	 * @return All particles.
	 */
	const std::map<std::shared_ptr<ParticleType>, std::vector<Particle>> &GetParticles() const { return m_particles; }

private:
	std::map<std::shared_ptr<ParticleType>, std::vector<Particle>> m_particles;
};
}
