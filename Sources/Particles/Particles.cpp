#include "Particles.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	const float Particles::MAX_ELAPSED_TIME = 5.0f;

	Particles::Particles() :
		IModule(),
		m_particles(std::map<std::shared_ptr<ParticleType>, std::vector<Particle *>>())
	{
	}

	Particles::~Particles()
	{
	}

	void Particles::Update()
	{
		if (Scenes::Get()->IsGamePaused())
		{
			return;
		}

		for (auto it = m_particles.begin(); it != m_particles.end(); ++it)
		{
			for (auto it1 = (*it).second.begin(); it1 != (*it).second.end(); ++it1)
			{
				(*it1)->Update();

				if (!(*it1)->IsAlive())
				{
					delete *it1;
					(*it).second.erase(it1);
				}
			}
		}
	}

	void Particles::AddParticle(Particle *created)
	{
		if (created == nullptr)
		{
			return;
		}

		auto list = m_particles.find(created->GetParticleType());

		if (list == m_particles.end())
		{
			m_particles.emplace(created->GetParticleType(), std::vector<Particle *>());
			list = m_particles.find(created->GetParticleType());
		}

		(*list).second.emplace_back(created);
	}

	void Particles::Clear()
	{
		m_particles.clear();
	}
}
