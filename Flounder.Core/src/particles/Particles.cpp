#include "Particles.hpp"

namespace Flounder
{
	const float Particles::MAX_ELAPSED_TIME = 5.0f;

	Particles::Particles() :
		IModule(),
		m_particleSystems(new std::vector<ParticleSystem*>()),
		m_particles(new std::map<particletype*, std::vector<Particle*>*>())
	{
	}

	Particles::~Particles()
	{
		delete m_particleSystems;
		delete m_particles;
	}

	void Particles::Update()
	{
		if (uis::get() != nullptr && uis::get()->getManager() != nullptr)
		{
			if (uis::get()->getManager()->IsGamePaused())
			{
				return;
			}
		}

		// Generates particles.
		for (auto system : *m_particleSystems)
		{
			Particle *created = system->GenerateParticles();

			if (created != nullptr)
			{
				std::vector<Particle*> *list = m_particles->find(created->GetParticleType())->second;

				if (list == nullptr)
				{
					list = new std::vector<Particle*>();
					m_particles->insert(std::pair<particletype*, std::vector<Particle*>*>(created->GetParticleType(), list));
				}

				if (list != nullptr)
				{
					list->push_back(created);
				}
			}
		}

		// Update and kill particles.
		for (auto iter = m_particles->begin(); iter != m_particles->end(); ++iter)
		{
			for (auto it = iter->second->begin(); it != iter->second->end(); ++it)
			{
				(*it)->update();

				if (!(*it)->isAlive())
				{
					delete *it;
					iter->second->erase(it);
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
