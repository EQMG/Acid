#include "Particles.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	Particles::Particles() :
		m_mutex(std::mutex()),
		m_particles(std::map<std::shared_ptr<ParticleType>, std::vector<Particle>>())
	{
	}

	void Particles::Update()
	{
		if (Scenes::Get()->IsPaused())
		{
			return;
		}

		std::lock_guard<std::mutex> lock(m_mutex);

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

	void Particles::AddParticle(const Particle &particle)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
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
		std::lock_guard<std::mutex> lock(m_mutex);
		m_particles.clear();
	}
}
