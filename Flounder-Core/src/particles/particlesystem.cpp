#include "particlesystem.h"

namespace flounder
{
	particlesystem::particlesystem(std::vector<particletype *> *types, ispawnparticle *spawn, const float &pps, const float &speed, const float &gravityEffect)
	{
		m_types = types;
		m_spawn = spawn;
		m_pps = pps;
		m_averageSpeed = speed;
		m_gravityEffect = gravityEffect;
		m_randomRotation = false;

		m_systemCentre = new vector3();
		m_velocityCentre = new vector3();

		m_timePassed = 0.0f;
		m_paused = false;

		//	particles::get()->addSystem(this);
	}

	particlesystem::~particlesystem()
	{
		//	particles::get()->removeSystem(this);
	}

	particle *particlesystem::generateParticles()
	{
		if (m_paused || m_types->empty())
		{
			return nullptr;
		}

		m_timePassed += framework::get()->getDelta();

		if (m_timePassed > (1.0f / m_pps))
		{
			m_timePassed = 0.0f;
			return emitParticle();
		}

		return nullptr;
	}

	particle *particlesystem::emitParticle()
	{
		vector3 *velocity;

		if (m_direction != 0)
		{
			velocity = generateRandomUnitVectorWithinCone(m_direction, m_directionDeviation);
		}
		else
		{
			velocity = generateRandomUnitVector();
		}

		particletype *emitType = m_types->at(static_cast<int>(floor(maths::randomInRange(0, static_cast<int>(m_types->size())))));

		velocity->normalize();
		velocity->scale(generateValue(m_averageSpeed, m_averageSpeed * maths::randomInRange(1.0f - m_speedError, 1.0f + m_speedError)));
		vector3::add(*velocity, *m_velocityCentre, velocity);
		float scale = generateValue(emitType->getScale(), emitType->getScale() * maths::randomInRange(1.0f - m_scaleError, 1.0f + m_scaleError));
		float lifeLength = generateValue(emitType->getLifeLength(), emitType->getLifeLength() * maths::randomInRange(1.0f - m_lifeError, 1.0f + m_lifeError));
		vector3 *spawnPos = vector3::add(*m_systemCentre, *m_spawn->getBaseSpawnPosition(), nullptr);

		particle *result = new particle(emitType, *spawnPos, *velocity, lifeLength, generateRotation(), scale, m_gravityEffect);

		delete velocity;
		delete spawnPos;

		return result;
	}

	float particlesystem::generateValue(const float &average, const float &errorMargin)
	{
		float offset = (maths::randomInRange(0.0f, 1.0f) - 0.5f) * 2.0f * errorMargin;
		return average + offset;
	}

	float particlesystem::generateRotation()
	{
		if (m_randomRotation)
		{
			return maths::randomInRange(0.0f, 360.0f);
		}

		return 0.0f;
	}

	vector3 *particlesystem::generateRandomUnitVectorWithinCone(vector3 *coneDirection, const float &angle)
	{
		float cosAngle = cos(angle);
		float theta = maths::randomInRange(0.0f, 1.0f) * 2.0f * PI;
		float z = cosAngle + maths::randomInRange(0.0f, 1.0f) * (1.0f - cosAngle);
		float rootOneMinusZSquared = sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);

		vector4 *direction = new vector4(x, y, z, 1.0f);

		if ((coneDirection->m_x != 0.0f) || (coneDirection->m_y != 0.0f) || ((coneDirection->m_z != 1.0f) && (coneDirection->m_z != -1.0f)))
		{
			vector3 *rotateAxis = vector3::cross(*coneDirection, vector3(0.0f, 0.0f, 1.0f), nullptr);
			rotateAxis->normalize();
			float rotateAngle = acos(vector3::dot(*coneDirection, vector3(0.0f, 0.0f, 1.0f)));
			matrix4x4 *rotationMatrix = new matrix4x4();
			matrix4x4::rotate(*rotationMatrix, *rotateAxis, -rotateAngle, rotationMatrix);
			matrix4x4::transform(*rotationMatrix, *direction, direction);

			delete rotateAxis;
			delete rotationMatrix;
		}
		else if (coneDirection->m_z == -1.0f)
		{
			direction->m_z *= -1.0f;
		}

		vector3 *result = new vector3(direction->m_x, direction->m_y, direction->m_z);
		delete direction;
		return result;
	}

	vector3 *particlesystem::generateRandomUnitVector()
	{
		float theta = maths::randomInRange(0.0f, 1.0f) * 2.0f * PI;
		float z = maths::randomInRange(0.0f, 1.0f) * 2.0f - 1.0f;
		float rootOneMinusZSquared = sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);
		return new vector3(x, y, z);
	}

	void particlesystem::addParticleType(particletype *type)
	{
		m_types->push_back(type);
	}

	void particlesystem::removeParticleType(particletype *type)
	{
		for (std::vector<particletype*>::iterator it = m_types->begin(); it != m_types->end(); ++it)
		{
			if (*it == type)
			{
				m_types->erase(it);
				return;
			}
		}
	}

	void particlesystem::setSpawn(ispawnparticle *spawn)
	{
		delete m_spawn;
		m_spawn = spawn;
	}

	void particlesystem::setDirection(const vector3 &direction, const float &deviation)
	{
		m_direction->set(direction);
		m_directionDeviation = static_cast<float>(deviation * PI);
	}
}
