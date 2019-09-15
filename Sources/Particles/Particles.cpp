#include "Particles.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
Particles::Particles()
{
}

void Particles::Update()
{
	if (Scenes::Get()->IsPaused())
	{
		return;
	}

	for (auto it = m_particles.begin(); it != m_particles.end();)
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

		if (it->second.empty())
		{
			it = m_particles.erase(it);
			continue;
		}

		std::sort((*it).second.begin(), (*it).second.end());
		(*it).first->Update((*it).second);
		++it;
	}
}

void Particles::AddParticle(Particle &&particle)
{
	auto it = m_particles.find(particle.GetParticleType());

	if (it == m_particles.end())
	{
		m_particles.emplace(particle.GetParticleType(), std::vector<Particle>());
		it = m_particles.find(particle.GetParticleType());
	}

	(*it).second.emplace_back(std::move(particle));
}

/*void Particles::RemoveParticle(const Particle &particle)
{
	auto it = m_particles.find(particle.GetParticleType());

	if (it != m_particles.end())
	{
		it->second.erase(std::remove_if(it->second.begin(), it->second.end(), [particle](Particle &p)
		{
			return p == particle;
		}), it->second.end());
	}
}*/

void Particles::Clear()
{
	m_particles.clear();
}
}
