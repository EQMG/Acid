#include "ParticleSystem.hpp"

#include "../Maths/Maths.hpp"
#include "Particles.hpp"

namespace Flounder
{
	ParticleSystem::ParticleSystem(std::vector<ParticleType *> *types, ISpawnParticle *spawn, const float &pps, const float &averageSpeed, const float &gravityEffect, const Vector3 &systemOffset) :
		Component(),
		m_types(types),
		m_spawn(spawn),
		m_pps(pps),
		m_averageSpeed(averageSpeed),
		m_gravityEffect(gravityEffect),
		m_randomRotation(false),
		m_lastPosition(new Vector3()),
		m_systemOffset(new Vector3(systemOffset)),
		m_direction(nullptr),
		m_directionDeviation(0.0f),
		m_speedError(0.0f),
		m_lifeError(0.0f),
		m_scaleError(0.0f),
		m_timePassed(0.0f),
		m_paused(false)
	{
	//	Link<std::vector<ParticleType *>>(0, "Types", LINK_GET(GetTypes()), LINK_SET(std::vector<ParticleType *>, SetTypes(v)));
	//	Link<ISpawnParticle *>(1, "Spawn", LINK_GET(GetSpawn()), LINK_SET(ISpawnParticle *, SetSpawn(v)));
		Link<float>(2, "PPS", LINK_GET(GetPps()), LINK_SET(float, SetPps(v)));
		Link<float>(3, "Average Speed", LINK_GET(GetAverageSpeed()), LINK_SET(float, SetAverageSpeed(v)));
		Link<float>(4, "Gravity Effect", LINK_GET(GetGravityEffect()), LINK_SET(float, SetGravityEffect(v)));
		Link<float>(5, "OffsetX", LINK_GET(GetOffsetCentre()->m_x), LINK_SET(float, GetOffsetCentre()->m_x = v));;
		Link<float>(6, "OffsetY", LINK_GET(GetOffsetCentre()->m_y), LINK_SET(float, GetOffsetCentre()->m_y = v));;
		Link<float>(7, "OffsetZ", LINK_GET(GetOffsetCentre()->m_z), LINK_SET(float, GetOffsetCentre()->m_z = v));
	}

	ParticleSystem::~ParticleSystem()
	{
		for (auto particleType : *m_types)
		{
			delete particleType;
		}

		delete m_types;
		delete m_spawn;

		delete m_lastPosition;

		delete m_systemOffset;

		delete m_direction;
	}

	void ParticleSystem::Update()
	{
		if (m_paused || m_types->empty())
		{
			return;
		}

		m_timePassed += Engine::Get()->GetDelta();

		if (m_timePassed > 1.0f / m_pps)
		{
			auto created = EmitParticle();
			Particles::Get()->AddParticle(created);
			m_timePassed = 0.0f;
		}
	}

	Particle *ParticleSystem::EmitParticle()
	{
		if (m_spawn == nullptr)
		{
			return nullptr;
		}

		Vector3 velocity = Vector3();
		float delta = Engine::Get()->GetDelta();
		Vector3::Subtract(*GetGameObject()->GetTransform()->GetPosition(), *m_lastPosition, &velocity);
		m_lastPosition->Set(*GetGameObject()->GetTransform()->GetPosition());
		velocity /= delta;

		if (m_direction != nullptr)
		{
			Vector3::RandomUnitVectorWithinCone(*m_direction, m_directionDeviation, &velocity);
		}
		else
		{
			GenerateRandomUnitVector(&velocity);
		}

		velocity.Normalize();
		velocity.Scale(GenerateValue(m_averageSpeed, m_averageSpeed * Maths::RandomInRange(1.0f - m_speedError, 1.0f + m_speedError)));

		ParticleType *emitType = m_types->at(static_cast<unsigned int>(std::floor(Maths::RandomInRange(0, static_cast<int>(m_types->size())))));
		float scale = GenerateValue(emitType->GetScale(), emitType->GetScale() * Maths::RandomInRange(1.0f - m_scaleError, 1.0f + m_scaleError));
		float lifeLength = GenerateValue(emitType->GetLifeLength(), emitType->GetLifeLength() * Maths::RandomInRange(1.0f - m_lifeError, 1.0f + m_lifeError));
		Vector3 spawnPos = Vector3();
		Vector3::Add(*GetGameObject()->GetTransform()->GetPosition(), *m_systemOffset, &spawnPos);
		Vector3::Add(spawnPos, *m_spawn->GetBaseSpawnPosition(), &spawnPos);
		return new Particle(emitType, spawnPos, velocity, lifeLength, GenerateRotation(), scale, m_gravityEffect);
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

	Vector3 *ParticleSystem::GenerateRandomUnitVector(Vector3 *destination) const
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		float theta = Maths::RandomInRange(0.0f, 1.0f) * 2.0f * PI;
		float z = Maths::RandomInRange(0.0f, 1.0f) * 2.0f - 1.0f;
		float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * std::cos(theta);
		float y = rootOneMinusZSquared * std::sin(theta);
		return destination->Set(x, y, z);
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
