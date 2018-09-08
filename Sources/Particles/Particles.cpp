#include "Particles.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	const float Particles::MAX_ELAPSED_TIME = 5.0f;

	Particles::Particles() :
		m_particles(std::map<std::shared_ptr<ParticleType>, std::vector<Particle>>())
	{
	}

	Particles::~Particles()
	{
	}

	void Particles::Update()
	{
		if (Scenes::Get()->IsPaused())
		{
			return;
		}

		for (auto it = m_particles.begin(); it != m_particles.end(); ++it)
		{
			for (auto it1 = (*it).second.begin(); it1 != (*it).second.end();)
			{
				(*it1).Update();

				if (!(*it1).IsAlive())
				{
					it1 = (*it).second.erase(it1);
					continue;
				}

				++it1;
			}
		}
	}

	void Particles::AddParticle(const Particle &particle)
	{
		auto it = m_particles.find(particle.GetParticleType());

		if (it == m_particles.end())
		{
			m_particles.emplace(particle.GetParticleType(), std::vector<Particle>());
			it = m_particles.find(particle.GetParticleType());
		}

		(*it).second.emplace_back(particle);
	}

	void Particles::Clear()
	{
		m_particles.clear();
	}
}
