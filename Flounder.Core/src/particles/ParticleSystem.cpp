#include "particlesystem.hpp"

namespace Flounder
{
	particlesystem::particlesystem(std::vector<particletype *> *types, ispawnparticle *spawn, const float &pps, const float &speed, const float &gravityEffect) :
		m_types(types),
		m_spawn(spawn),
		m_pps(pps),
		m_averageSpeed(speed),
		m_gravityEffect(gravityEffect),
		m_randomRotation(false),
		m_systemCentre(new Vector3()),
		m_velocityCentre(new Vector3()),
		m_timePassed(0.0f),
		m_paused(false)
	{
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

		m_timePassed += Engine::Get()->GetDelta();

		if (m_timePassed > (1.0f / m_pps))
		{
			m_timePassed = 0.0f;
			return emitParticle();
		}

		return nullptr;
	}

	particle *particlesystem::emitParticle()
	{
		Vector3 *velocity;

		if (m_direction != 0)
		{
			velocity = generateRandomUnitVectorWithinCone(m_direction, m_directionDeviation);
		}
		else
		{
			velocity = generateRandomUnitVector();
		}

		particletype *emitType = m_types->at(static_cast<int>(floor(Maths::RandomInRange(0, static_cast<int>(m_types->size())))));

		velocity->normalize();
		velocity->scale(generateValue(m_averageSpeed, m_averageSpeed * Maths::RandomInRange(1.0f - m_speedError, 1.0f + m_speedError)));
		Vector3::add(*velocity, *m_velocityCentre, velocity);
		float scale = generateValue(emitType->getScale(), emitType->getScale() * Maths::RandomInRange(1.0f - m_scaleError, 1.0f + m_scaleError));
		float lifeLength = generateValue(emitType->getLifeLength(), emitType->getLifeLength() * Maths::RandomInRange(1.0f - m_lifeError, 1.0f + m_lifeError));
		Vector3 *spawnPos = Vector3::add(*m_systemCentre, *m_spawn->getBaseSpawnPosition(), nullptr);

		particle *result = new particle(emitType, *spawnPos, *velocity, lifeLength, generateRotation(), scale, m_gravityEffect);

		delete velocity;
		delete spawnPos;

		return result;
	}

	float particlesystem::generateValue(const float &average, const float &errorMargin)
	{
		float offset = (Maths::RandomInRange(0.0f, 1.0f) - 0.5f) * 2.0f * errorMargin;
		return average + offset;
	}

	float particlesystem::generateRotation()
	{
		if (m_randomRotation)
		{
			return Maths::RandomInRange(0.0f, 360.0f);
		}

		return 0.0f;
	}

	Vector3 *particlesystem::generateRandomUnitVectorWithinCone(Vector3 *coneDirection, const float &angle)
	{
		float cosAngle = cos(angle);
		float theta = Maths::RandomInRange(0.0f, 1.0f) * 2.0f * PI;
		float z = cosAngle + Maths::RandomInRange(0.0f, 1.0f) * (1.0f - cosAngle);
		float rootOneMinusZSquared = sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);

		Vector4 *direction = new Vector4(x, y, z, 1.0f);

		if ((coneDirection->m_x != 0.0f) || (coneDirection->m_y != 0.0f) || ((coneDirection->m_z != 1.0f) && (coneDirection->m_z != -1.0f)))
		{
			Vector3 *rotateAxis = Vector3::cross(*coneDirection, Vector3(0.0f, 0.0f, 1.0f), nullptr);
			rotateAxis->normalize();
			float rotateAngle = acos(Vector3::dot(*coneDirection, Vector3(0.0f, 0.0f, 1.0f)));
			Matrix4 *rotationMatrix = new Matrix4();
			Matrix4::rotate(*rotationMatrix, *rotateAxis, -rotateAngle, rotationMatrix);
			Matrix4::transform(*rotationMatrix, *direction, direction);

			delete rotateAxis;
			delete rotationMatrix;
		}
		else if (coneDirection->m_z == -1.0f)
		{
			direction->m_z *= -1.0f;
		}

		Vector3 *result = new Vector3(direction->m_x, direction->m_y, direction->m_z);
		delete direction;
		return result;
	}

	Vector3 *particlesystem::generateRandomUnitVector()
	{
		float theta = Maths::RandomInRange(0.0f, 1.0f) * 2.0f * PI;
		float z = Maths::RandomInRange(0.0f, 1.0f) * 2.0f - 1.0f;
		float rootOneMinusZSquared = sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);
		return new Vector3(x, y, z);
	}

	void particlesystem::addParticleType(particletype *type)
	{
		m_types->push_back(type);
	}

	void particlesystem::removeParticleType(particletype *type)
	{
		for (auto it = m_types->begin(); it != m_types->end(); ++it)
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

	void particlesystem::setDirection(const Vector3 &direction, const float &deviation)
	{
		m_direction->set(direction);
		m_directionDeviation = static_cast<float>(deviation * PI);
	}
}
