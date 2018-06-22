#include "ParticleSystem.hpp"

#include "Maths/Maths.hpp"
#include "Particles.hpp"

namespace fl
{
	ParticleSystem::ParticleSystem(const std::vector<std::shared_ptr<ParticleType>> &types, ISpawnParticle *spawn, const float &pps, const float &averageSpeed, const float &gravityEffect, const Vector3 &systemOffset) :
		IComponent(),
		m_types(types),
		m_spawn(spawn),
		m_pps(pps),
		m_averageSpeed(averageSpeed),
		m_gravityEffect(gravityEffect),
		m_randomRotation(false),
		m_lastPosition(Vector3()),
		m_systemOffset(Vector3(systemOffset)),
		m_direction(Vector3()),
		m_directionDeviation(0.0f),
		m_speedError(0.0f),
		m_lifeError(0.0f),
		m_scaleError(0.0f),
		m_timePassed(0.0f),
		m_paused(false)
	{
	}

	ParticleSystem::~ParticleSystem()
	{
		delete m_spawn;
	}

	void ParticleSystem::Update()
	{
		if (m_paused || m_types.empty())
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

	void ParticleSystem::Load(LoadedValue *value)
	{
		//	Link<std::vector<ParticleType *>>(0, "Types", LINK_GET(GetTypes()), LINK_SET(std::vector<ParticleType *>, SetTypes(v)));
		//	Link<ISpawnParticle *>(1, "Spawn", LINK_GET(GetSpawn()), LINK_SET(ISpawnParticle *, SetSpawn(v)));
		m_pps = value->GetChild("PPS")->Get<float>();
		m_averageSpeed = value->GetChild("Average Speed")->Get<float>();
		m_gravityEffect = value->GetChild("Gravity Effect")->Get<float>();
		m_systemOffset = value->GetChild("Offset");
	}

	void ParticleSystem::Write(LoadedValue *destination)
	{
		destination->GetChild("PPS", true)->Set(m_pps);
		destination->GetChild("Average Speed", true)->Set(m_averageSpeed);
		destination->GetChild("Gravity Effect", true)->Set(m_gravityEffect);
		m_systemOffset.Write(destination->GetChild("Offset", true));
	}

	Particle *ParticleSystem::EmitParticle()
	{
		if (m_spawn == nullptr)
		{
			return nullptr;
		}

		Vector3 velocity = Vector3();
		float delta = Engine::Get()->GetDelta();
		velocity = GetGameObject()->GetTransform()->GetPosition() - m_lastPosition;
		m_lastPosition = GetGameObject()->GetTransform()->GetPosition();
		velocity /= delta;

		if (m_direction == 0.0f)
		{
			velocity = Vector3::RandomUnitVectorWithinCone(m_direction, m_directionDeviation);
		}
		else
		{
			velocity = GenerateRandomUnitVector();
		}

		velocity = velocity.Normalize();
		velocity *= GenerateValue(m_averageSpeed, m_averageSpeed * Maths::Random(1.0f - m_speedError, 1.0f + m_speedError));

		auto emitType = m_types.at(static_cast<unsigned int>(std::floor(Maths::Random(0, static_cast<int>(m_types.size())))));
		float scale = GenerateValue(emitType->GetScale(), emitType->GetScale() * Maths::Random(1.0f - m_scaleError, 1.0f + m_scaleError));
		float lifeLength = GenerateValue(emitType->GetLifeLength(), emitType->GetLifeLength() * Maths::Random(1.0f - m_lifeError, 1.0f + m_lifeError));
		Vector3 spawnPos = Vector3();
		spawnPos = GetGameObject()->GetTransform()->GetPosition() + m_systemOffset;
		spawnPos = spawnPos + m_spawn->GetBaseSpawnPosition();
		return new Particle(emitType, spawnPos, velocity, lifeLength, GenerateRotation(), scale, m_gravityEffect);
	}

	float ParticleSystem::GenerateValue(const float &average, const float &errorMargin) const
	{
		return average + ((Maths::Random(0.0f, 1.0f) - 0.5f) * 2.0f * errorMargin);
	}

	float ParticleSystem::GenerateRotation() const
	{
		if (m_randomRotation)
		{
			return Maths::Random(0.0f, 360.0f);
		}

		return 0.0f;
	}

	Vector3 ParticleSystem::GenerateRandomUnitVector() const
	{
		float theta = Maths::Random(0.0f, 1.0f) * 2.0f * PI;
		float z = Maths::Random(0.0f, 1.0f) * 2.0f - 1.0f;
		float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * std::cos(theta);
		float y = rootOneMinusZSquared * std::sin(theta);
		return Vector3(x, y, z);
	}

	void ParticleSystem::AddParticleType(std::shared_ptr<ParticleType> type)
	{
		m_types.emplace_back(type);
	}

	void ParticleSystem::RemoveParticleType(std::shared_ptr<ParticleType> type)
	{
		for (auto it = m_types.begin(); it != m_types.end(); ++it)
		{
			if (*it == type)
			{
				m_types.erase(it);
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
		m_direction = direction;
		m_directionDeviation = deviation * PI;
	}
}
