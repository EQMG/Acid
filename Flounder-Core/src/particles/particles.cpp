#include "particles.h"

namespace flounder
{
	const float particles::MAX_ELAPSED_TIME = 5.0f;

	particles::particles() :
		imodule()
	{
		m_particleSystems = new std::vector<particlesystem*>();
		m_particles = new std::map<particletype*, std::vector<particle*>*>();
	}

	particles::~particles()
	{
		delete m_particleSystems;
		delete m_particles;
	}
	
	void particles::update()
	{
		if (uis::get()->getManager()->isGamePaused())
		{
			return;
		}

		// Generate particles.
		/*for (particlesystem* system : *m_particleSystems)
		{
			particle *created = system->generateParticles();

			if (created != NULL)
			{
				std::vector<particle*> *list = m_particles->find(created->getParticleType())->second;
				
				if (list == NULL)
				{
					list = new std::vector<particle*>();
					m_particles->insert(std::pair<particletype*, std::vector<particle*>*>(created->getParticleType(), list));
				}

				list->push_back(created);
			}
		}*/

		// Update and kill particles.
		for(std::map<particletype*, std::vector<particle*>*>::iterator iter = m_particles->begin(); iter != m_particles->end(); ++iter)
		{
			for (std::vector<particle*>::iterator it = iter->second->begin(); it != iter->second->end(); ++it)
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
		for (std::vector<particlesystem*>::iterator it = m_particleSystems->begin(); it != m_particleSystems->end(); ++it)
		{
			if (*it == system)
			{
				m_particleSystems->erase(it);
				return;
			}
		}
	}
}
