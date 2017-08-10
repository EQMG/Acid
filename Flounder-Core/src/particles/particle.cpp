#include "particle.h"

namespace flounder
{
	particle::particle(particletype *particleType, const vector3 &position, const vector3 &velocity, const float &lifeLength, const float &rotation, const float &scale, const float &gravityEffect)
	{
		m_particleType = particleType;
		m_position = new vector3(position);
		m_velocity = new vector3(velocity);
		m_change = new vector3();

		m_textureOffset1 = new vector2();
		m_textureOffset2 = new vector2();

		m_lifeLength = lifeLength;
		m_rotation = rotation;
		m_scale = scale;
		m_gravityEffect = gravityEffect;

		m_elapsedTime = 0.0f;
		m_transparency = 0.0f;
		m_textureBlendFactor = 0.0f;
		m_distanceToCamera = 0.0f;
	}

	particle::~particle()
	{
		delete m_position;
		delete m_velocity;
		delete m_change;

		delete m_textureOffset1;
		delete m_textureOffset2;
	}

	void particle::update()
	{
		m_velocity->m_y += -10.0f * m_gravityEffect * framework::get()->getDelta();
		m_change->set(*m_velocity);
		m_change->scale(framework::get()->getDelta());

		vector3::add(*m_change, *m_position, m_position);
		m_elapsedTime += framework::get()->getDelta();

		if (m_elapsedTime > m_lifeLength)
		{
			m_transparency += 1.0f * framework::get()->getDelta();
		}

		if (!isAlive() || camera::get()->getCamera() == nullptr)
		{
			return;
		}

		vector3 *cameraToParticle = vector3::subtract(*camera::get()->getCamera()->getPosition(), *m_position, nullptr);
		m_distanceToCamera = cameraToParticle->lengthSquared();
		delete cameraToParticle;

		float lifeFactor = m_elapsedTime / m_lifeLength;

		if (m_particleType->getTexture() == nullptr)
		{
			return;
		}

		int stageCount = static_cast<int>(pow(m_particleType->getTexture()->getNumberOfRows(), 2));
		float atlasProgression = lifeFactor * stageCount;
		int index1 = static_cast<int>(floor(atlasProgression));
		int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;

		m_textureBlendFactor = fmod(atlasProgression, 1.0f);
		updateTextureOffset(m_textureOffset1, index1);
		updateTextureOffset(m_textureOffset2, index2);
	}

	bool particle::operator<(const particle &other) const
	{
		return m_distanceToCamera > other.m_distanceToCamera;
	}

	vector2 *particle::updateTextureOffset(vector2 *offset, const int &index)
	{
		offset->set(0.0f, 0.0f);
		int column = index % m_particleType->getTexture()->getNumberOfRows();
		int row = index / m_particleType->getTexture()->getNumberOfRows();
		offset->m_x = static_cast<float>(column) / m_particleType->getTexture()->getNumberOfRows();
		offset->m_y = static_cast<float>(row) / m_particleType->getTexture()->getNumberOfRows();
		return offset;
	}
}
