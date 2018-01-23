#include "ParticleSystem.hpp"

#include "../Maths/Maths.hpp"

namespace Flounder
{
	ParticleSystem::ParticleSystem(std::vector<ParticleType *> *types, ISpawnParticle *spawn, const float &pps, const float &speed, const float &gravityEffect) :
		m_types(types),
		m_spawn(spawn),
		m_pps(pps),
		m_averageSpeed(speed),
		m_gravityEffect(gravityEffect),
		m_randomRotation(false),
		m_systemCentre(new Vector3()),
		m_velocityCentre(new Vector3()),
		m_direction(nullptr),
		m_directionDeviation(0.0f),
		m_speedError(0.0f),
		m_lifeError(0.0f),
		m_scaleError(0.0f),
		m_timePassed(0.0f),
		m_paused(false)
	{
		//	particles::get()->addSystem(this);
	}

	ParticleSystem::~ParticleSystem()
	{
		//	particles::get()->removeSystem(this);
	}

	Particle *ParticleSystem::GenerateParticles()
	{
		if (m_paused || m_types->empty())
		{
			return nullptr;
		}

		m_timePassed += Engine::Get()->GetDelta();

		if (m_timePassed > (1.0f / m_pps))
		{
			m_timePassed = 0.0f;
			return EmitParticle();
		}

		return nullptr;
	}

	Particle *ParticleSystem::EmitParticle()
	{
		Vector3 *velocity;

		if (m_direction != nullptr)
		{
			velocity = Vector3::RandomUnitVectorWithinCone(*m_direction, m_directionDeviation, nullptr);
		}
		else
		{
			velocity = GenerateRandomUnitVector();
		}

		ParticleType *emitType = m_types->at(static_cast<unsigned int>(std::floor(Maths::RandomInRange(0, static_cast<int>(m_types->size())))));

		velocity->Normalize();
		velocity->Scale(GenerateValue(m_averageSpeed, m_averageSpeed * Maths::RandomInRange(1.0f - m_speedError, 1.0f + m_speedError)));
		Vector3::Add(*velocity, *m_velocityCentre, velocity);
		float scale = GenerateValue(emitType->GetScale(), emitType->GetScale() * Maths::RandomInRange(1.0f - m_scaleError, 1.0f + m_scaleError));
		float lifeLength = GenerateValue(emitType->GetLifeLength(), emitType->GetLifeLength() * Maths::RandomInRange(1.0f - m_lifeError, 1.0f + m_lifeError));
		Vector3 *spawnPos = Vector3::Add(*m_systemCentre, *m_spawn->GetBaseSpawnPosition(), nullptr);

		Particle *result = new Particle(emitType, *spawnPos, *velocity, lifeLength, GenerateRotation(), scale, m_gravityEffect);

		delete velocity;
		delete spawnPos;

		return result;
	}

	float ParticleSystem::GenerateValue(const float &average, const float &errorMargin) const
	{
		return average + ((Maths::RandomInRange(0.0f, 1.0f) - 0.5f) * 2.0f * errorMargin);
	}

	float ParticleSystem::GenerateRotation() const
	{
		if (m_randomRotation)
		{
			return Maths::RandomInRange(0.0f, 360.0f);
		}

		return 0.0f;
	}

	Vector3 *ParticleSystem::GenerateRandomUnitVector() const
	{
		float theta = Maths::RandomInRange(0.0f, 1.0f) * 2.0f * PI;
		float z = Maths::RandomInRange(0.0f, 1.0f) * 2.0f - 1.0f;
		float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * std::cos(theta);
		float y = rootOneMinusZSquared * std::sin(theta);
		return new Vector3(x, y, z);
	}

	void ParticleSystem::AddParticleType(ParticleType *type) const
	{
		m_types->push_back(type);
	}

	void ParticleSystem::RemoveParticleType(ParticleType *type) const
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

	void ParticleSystem::SetSpawn(ISpawnParticle *spawn)
	{
		delete m_spawn;
		m_spawn = spawn;
	}

	void ParticleSystem::SetDirection(const Vector3 &direction, const float &deviation)
	{
		m_direction->Set(direction);
		m_directionDeviation = static_cast<float>(deviation * PI);
	}
}
