#include "Particles.hpp"

#include "../Scenes/Scenes.hpp"

namespace Flounder
{
	const float Particles::MAX_ELAPSED_TIME = 5.0f;

	Particles::Particles() :
		IModule(),
		m_particleSystems(new std::vector<ParticleSystem *>()),
		m_particles(new std::map<ParticleType *, std::vector<Particle *> *>())
	{
	}

	Particles::~Particles()
	{
		delete m_particleSystems;
		delete m_particles;
	}

	void Particles::Update()
	{
		if (Scenes::Get()->IsGamePaused())
		{
			return;
		}

		// Generates particles.
		for (auto system : *m_particleSystems)
		{
			Particle *created = system->GenerateParticle();

			if (created != nullptr)
			{
				auto list = m_particles->find(created->GetParticleType());

				if (list == m_particles->end())
				{
					m_particles->insert(std::make_pair(created->GetParticleType(), new std::vector<Particle *>()));
					list = m_particles->find(created->GetParticleType());
				}

				if ((*list).second != nullptr)
				{
					(*list).second->push_back(created);
				}
			}
		}

		// Update and kill particles.
		for (auto &m_particle : *m_particles)
		{
			for (auto it = m_particle.second->begin(); it != m_particle.second->end(); ++it)
			{
				(*it)->Update();

				if (!(*it)->IsAlive())
				{
					delete *it;
					m_particle.second->erase(it);
				}
			}
		}
	}

	void Particles::Clear()
	{
		m_particles->clear();
	}

	void Particles::AddSystem(ParticleSystem *system)
	{
		m_particleSystems->push_back(system);
	}

	void Particles::RemoveSystem(ParticleSystem *system)
	{
		for (auto it = m_particleSystems->begin(); it != m_particleSystems->end(); ++it)
		{
			if (*it == system)
			{
				m_particleSystems->erase(it);
				return;
			}
		}
	}
}
