#include "particles.h"

namespace flounder
{
	const float particles::MAX_ELAPSED_TIME = 5.0f;

	particles::particles() :
		imodule()
	{
		m_particleSystems = new std::vector<particlesystem*>();
		m_particles = new std::vector<structurebasic<particle*>*>();
		m_deadParticles = new std::vector<particle*>();
	}

	particles::~particles()
	{
		delete m_particleSystems;
		delete m_particles;
		delete m_deadParticles;
	}
	
	void particles::update()
	{
		/*//	if (guis::get()->getGuiMaster()->isGamePaused())
		//	{
		//		return;
		//	}
	
			// Generate particles.
			for (particlesystem* system : *m_particleSystems)
			{
				system->generateParticles();
			}
	
			// Update particles.
			for (structurebasic<particle*> *structure : *m_particles)
			{
				structurebasic<particle*> particleStructure = *structure;
	
				while (particleIterator != m_particles->end())
				{
					particle *object = *particleIterator;
					object->update();
	
					if (!object->isAlive())
					{
						(*particleIterator)->remove();
						m_deadParticles->push_back(object);
					}
					particleIterator++;
				}
			}
	
			// Update dead particle objects.
			if (!m_deadParticles->empty())
			{
				std::vector<particle*>::const_iterator deadIterator = m_deadParticles->begin();
	
				while (deadIterator != m_deadParticles->end())
				{
					particle *object = *deadIterator;
					object->update();
	
					if (object->getElapsedTime() > MAX_ELAPSED_TIME)
					{
						(*deadIterator)->remove();
					}
	
					deadIterator++;
				}
	
				std::sort(m_deadParticles->begin(), m_deadParticles->end()); // Sorts the list old to new.
			}*/
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
		//	m_particleSystems->remove(system);
	}

	void particles::addParticle(particletype *particleType, const vector3 &position, const vector3 &velocity, const float &lifeLength, const float &rotation, const float &scale, const float &gravityEffect)
	{
		/*	particle *result = NULL;
	
			if (m_deadParticles->size() > 0)
			{
				result = m_deadParticles->at(0)->set(particleType, position, velocity, lifeLength, rotation, scale, gravityEffect);
				m_deadParticles->remove(0);
			}
			else
			{
				result = new particle(particleType, position, velocity, lifeLength, rotation, scale, gravityEffect);
			}
	
			for (std::vector<structurebasic<particle*>*>::const_iterator list = m_particles->begin(); list != m_particles->end(); ++list)
			{
				//JAVA TO C++ CONVERTER TODO TASK: The following line could not be converted:
				if ((*list)->getSize() > 0 && (*list)->at(0).getParticleType().equals(result.getParticleType()))
				{
					(*list)->add(result);
					return;
				}
			}
	
			structurebasic<particle*> *list = new structurebasic<particle*>();
			list->add(result);
			m_particles->push_back(list);*/
	}
}
