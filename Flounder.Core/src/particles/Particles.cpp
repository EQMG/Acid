#include "particles.hpp"

namespace Flounder
{
	const float particles::MAX_ELAPSED_TIME = 5.0f;

	particles::particles() :
		IModule(),
		m_particleSystems(new std::vector<particlesystem*>()),
		m_particles(new std::map<particletype*, std::vector<particle*>*>())
	{
	}

	particles::~particles()
	{
		delete m_particleSystems;
		delete m_particles;
	}

	void particles::Update()
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
			particle *created = system->generateParticles();

			if (created != nullptr)
			{
				std::vector<particle*> *list = m_particles->find(created->getParticleType())->second;

				if (list == nullptr)
				{
					list = new std::vector<particle*>();
					m_particles->insert(std::pair<particletype*, std::vector<particle*>*>(created->getParticleType(), list));
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

	void particles::clear()
	{
		m_particles->clear();
	}

	void particles::addSystem(particlesystem *system)
	{
		m_particleSystems->push_back(system);
	}

	void particles::removeSystem(particlesystem *system)
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
