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
	public Module
{
public:
	/// <summary>
	/// Gets this engine instance.
	/// </summary>
	/// <returns> The current module instance. </returns>
	static Particles *Get() { return Engine::Get()->GetModuleManager().Get<Particles>(); }

	Particles();

	void Update() override;

	void AddParticle(const Particle &particle);

	//void RemoveParticle(const Particle &particle);

	/// <summary>
	/// Clears all particles from the scene.
	/// </summary>
	void Clear();

	/// <summary>
	/// Gets a list of all particles.
	/// </summary>
	/// <returns> All particles. </returns>
	const std::map<std::shared_ptr<ParticleType>, std::vector<Particle>> &GetParticles() const { return m_particles; }

private:
	std::map<std::shared_ptr<ParticleType>, std::vector<Particle>> m_particles;
};
}
