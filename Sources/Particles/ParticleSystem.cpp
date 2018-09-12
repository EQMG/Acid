#include "ParticleSystem.hpp"

#include "Maths/Maths.hpp"
#include "Particles/Spawns/SpawnCircle.hpp"
#include "Particles/Spawns/SpawnLine.hpp"
#include "Particles/Spawns/SpawnPoint.hpp"
#include "Particles/Spawns/SpawnSphere.hpp"
#include "Particles.hpp"

namespace acid
{
	ParticleSystem::ParticleSystem(const std::vector<std::shared_ptr<ParticleType>> &types, ISpawnParticle *spawn, const float &pps, const float &averageSpeed, const float &gravityEffect, const Vector3 &systemOffset) :
		m_types(types),
		m_spawn(spawn),
		m_pps(pps),
		m_averageSpeed(averageSpeed),
		m_gravityEffect(gravityEffect),
		m_randomRotation(false),
		m_lastPosition(Vector3()),
		m_systemOffset(systemOffset),
		m_direction(Vector3()),
		m_directionDeviation(0.0f),
		m_speedError(0.0f),
		m_lifeError(0.0f),
		m_scaleError(0.0f),
		m_timePassed(0.0f),
		m_paused(false)
	{
	}

	void ParticleSystem::Start()
	{
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

			if (created)
			{
				Particles::Get()->AddParticle(*created);
				m_timePassed = 0.0f;
			}
		}
	}

	void ParticleSystem::Decode(const Metadata &metadata)
	{
		for (auto &typeNode : metadata.FindChild("Types")->GetChildren())
		{
			ParticleType temp = ParticleType();
			temp.Decode(*typeNode);
			m_types.emplace_back(ParticleType::Resource(temp.GetFilename()));
		}

		TrySetSpawn(*metadata.FindChild("Spawn"));

		m_pps = metadata.GetChild<float>("PPS");
		m_averageSpeed = metadata.GetChild<float>("Average Speed");
		m_gravityEffect = metadata.GetChild<float>("Gravity Effect");
		m_systemOffset = metadata.GetChild<Vector3>("Offset");
	}

	void ParticleSystem::Encode(Metadata &metadata) const
	{
		auto typesNode = metadata.FindChild("Types");

		if (typesNode == nullptr)
		{
			typesNode = metadata.AddChild(new Metadata("Types"));
		}

		for (auto &type : m_types)
		{
			type->Encode(*typesNode->AddChild(new Metadata()));
		}

		metadata.SetChild<ISpawnParticle>("Spawn", *m_spawn);

		metadata.SetChild<float>("PPS", m_pps);
		metadata.SetChild<float>("Average Speed", m_averageSpeed);
		metadata.SetChild<float>("Gravity Effect", m_gravityEffect);
		metadata.SetChild<Vector3>("Offset", m_systemOffset);
		// TODO: m_randomRotation, m_direction, m_directionDeviation, m_speedError, m_lifeError, m_scaleError
	}

	void ParticleSystem::AddParticleType(const std::shared_ptr<ParticleType> &type)
	{
		if (std::find(m_types.begin(), m_types.end(), type) != m_types.end())
		{
			return;
		}

		m_types.emplace_back(type);
	}

	bool ParticleSystem::RemoveParticleType(const std::shared_ptr<ParticleType> &type)
	{
		for (auto it = m_types.begin(); it != m_types.end(); ++it)
		{
			if (*it == type)
			{
				m_types.erase(it);
				return true;
			}
		}

		return false;
	}

	void ParticleSystem::TrySetSpawn(const Metadata &spawnNode)
	{
		// TODO: Modularize.
		std::string spawnName = spawnNode.GetChild<std::string>("Type");

		if (spawnName == "SpawnCircle")
		{
			m_spawn = std::make_unique<SpawnCircle>();
			m_spawn->Decode(spawnNode);
			return;
		}

		if (spawnName == "SpawnLine")
		{
			m_spawn = std::make_unique<SpawnLine>();
			m_spawn->Decode(spawnNode);
			return;
		}

		if (spawnName == "SpawnPoint")
		{
			m_spawn = std::make_unique<SpawnPoint>();
			m_spawn->Decode(spawnNode);
			return;
		}

		if (spawnName == "SpawnSphere")
		{
			m_spawn = std::make_unique<SpawnSphere>();
			m_spawn->Decode(spawnNode);
			return;
		}

		Log::Error("Could not determine particle spawn type: '%s'\n", spawnName.c_str());
	}

	void ParticleSystem::SetDirection(const Vector3 &direction, const float &deviation)
	{
		m_direction = direction;
		m_directionDeviation = deviation * PI;
	}

	std::optional<Particle> ParticleSystem::EmitParticle()
	{
		if (m_spawn == nullptr)
		{
			return {};
		}

		Vector3 velocity = Vector3();
		float delta = Engine::Get()->GetDelta();
		velocity = GetGameObject()->GetTransform().GetPosition() - m_lastPosition;
		m_lastPosition = GetGameObject()->GetTransform().GetPosition();
		velocity /= delta;

		if (m_direction != 0.0f)
		{
			velocity = Vector3::RandomUnitVectorWithinCone(m_direction, m_directionDeviation);
		}
		else
		{
			velocity = GenerateRandomUnitVector();
		}

		velocity = velocity.Normalize();
		velocity *= GenerateValue(m_averageSpeed, m_averageSpeed * Maths::Random(1.0f - m_speedError, 1.0f + m_speedError));

		auto emitType = m_types.at(static_cast<uint32_t>(std::floor(Maths::Random(0, static_cast<int32_t>(m_types.size())))));
		float scale = GenerateValue(emitType->GetScale(), emitType->GetScale() * Maths::Random(1.0f - m_scaleError, 1.0f + m_scaleError));
		float lifeLength = GenerateValue(emitType->GetLifeLength(), emitType->GetLifeLength() * Maths::Random(1.0f - m_lifeError, 1.0f + m_lifeError));

		Vector3 spawnPos;
		spawnPos = GetGameObject()->GetTransform().GetPosition() + m_systemOffset;
		spawnPos = spawnPos + m_spawn->GetBaseSpawnPosition();
		return Particle(emitType, spawnPos, velocity, lifeLength, GenerateRotation(), scale, m_gravityEffect);
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
}
