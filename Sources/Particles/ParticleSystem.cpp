#include "ParticleSystem.hpp"

#include "Maths/Maths.hpp"
#include "Particles.hpp"

namespace acid
{
	ParticleSystem::ParticleSystem(const std::vector<std::shared_ptr<ParticleType>> &types, const float &pps, const float &averageSpeed, const float &gravityEffect) :
		m_types(types),
		m_pps(pps),
		m_averageSpeed(averageSpeed),
		m_gravityEffect(gravityEffect),
		m_randomRotation(false),
		m_direction(Vector3()),
		m_directionDeviation(0.0f),
		m_speedDeviation(0.0f),
		m_lifeDeviation(0.0f),
		m_stageDeviation(0.0f),
		m_scaleDeviation(0.0f),
		m_emitTimer(Timer(Time::Seconds(1.0f / m_pps)))
	{
	}

	void ParticleSystem::Start()
	{
	}

	void ParticleSystem::Update()
	{
		if (m_types.empty())
		{
			return;
		}

		if (m_emitTimer.IsPassedTime())
		{
			float pastFactor = std::floor(m_emitTimer.GetDifference() / m_emitTimer.GetInterval());
			m_emitTimer.ResetStartTime();

			auto emitters = GetParent()->GetComponents<ParticleEmitter>();

			if (emitters.empty())
			{
				return;
			}

			for (uint32_t i = 0; i < pastFactor; i++)
			{
				Particles::Get()->AddParticle(EmitParticle(*emitters[static_cast<uint32_t>(Maths::Random(0, emitters.size()))]));
			}
		}
	}

	void ParticleSystem::Decode(const Metadata &metadata)
	{
		auto typesNode = metadata.FindChild("Types");

		if (typesNode != nullptr)
		{
			for (auto &typeNode : typesNode->GetChildren())
			{
				ParticleType temp = ParticleType();
				temp.Decode(*typeNode);
				m_types.emplace_back(ParticleType::Resource(temp.GetFilename()));
			}
		}

		m_pps = metadata.GetChild<float>("PPS");
		m_emitTimer = Timer(Time::Seconds(1.0f / m_pps));
		m_averageSpeed = metadata.GetChild<float>("Average Speed");
		m_gravityEffect = metadata.GetChild<float>("Gravity Effect");
		m_randomRotation = metadata.GetChild<bool>("Random Rotation");
		m_direction = metadata.GetChild<Vector3>("Direction");
		m_directionDeviation = metadata.GetChild<float>("Direction Deviation");
		m_speedDeviation = metadata.GetChild<float>("Speed Deviation");
		m_lifeDeviation = metadata.GetChild<float>("Life Deviation");
		m_stageDeviation = metadata.GetChild<float>("Stage Deviation");
		m_scaleDeviation = metadata.GetChild<float>("Scale Deviation");
	}

	void ParticleSystem::Encode(Metadata &metadata) const
	{
		auto typesNode = metadata.FindChild("Types", false);

		if (typesNode == nullptr)
		{
			typesNode = metadata.AddChild(new Metadata("Types"));
		}

		for (auto &type : m_types)
		{
			type->Encode(*typesNode->AddChild(new Metadata()));
		}

		metadata.SetChild<float>("PPS", m_pps);
		metadata.SetChild<float>("Average Speed", m_averageSpeed);
		metadata.SetChild<float>("Gravity Effect", m_gravityEffect);
		metadata.SetChild<bool>("Random Rotation", m_randomRotation);
		metadata.SetChild<Vector3>("Direction", m_direction);
		metadata.SetChild<float>("Direction Deviation", m_directionDeviation);
		metadata.SetChild<float>("Speed Deviation", m_speedDeviation);
		metadata.SetChild<float>("Life Deviation", m_lifeDeviation);
		metadata.SetChild<float>("Stage Deviation", m_stageDeviation);
		metadata.SetChild<float>("Scale Deviation", m_scaleDeviation);
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

	void ParticleSystem::SetPps(const float &pps)
	{
		m_pps = pps;
		m_emitTimer = Timer(Time::Seconds(1.0f / m_pps));
	}

	void ParticleSystem::SetDirection(const Vector3 &direction, const float &deviation)
	{
		m_direction = direction;
		m_directionDeviation = deviation * PI;
	}

	Particle ParticleSystem::EmitParticle(const ParticleEmitter &emitter)
	{
		auto worldTransform = GetParent()->GetWorldTransform() * emitter.GetLocalTransform();
		Vector3 spawnPos = emitter.GeneratePosition() + worldTransform.GetPosition();

		Vector3 velocity;

		if (m_direction != 0.0f)
		{
			velocity = Vector3::RandomUnitVectorWithinCone(m_direction, m_directionDeviation);
		}
		else
		{
			velocity = GenerateRandomUnitVector();
		}

		velocity = velocity.Normalize();
		velocity *= GenerateValue(m_averageSpeed, m_speedDeviation);

		auto emitType = m_types.at(static_cast<uint32_t>(std::floor(Maths::Random(0.0f, static_cast<float>(m_types.size())))));
		float scale = GenerateValue(emitType->GetScale(), m_scaleDeviation);
		float lifeLength = GenerateValue(emitType->GetLifeLength(), m_lifeDeviation);
		float stageCycles = GenerateValue(emitType->GetStageCycles(), m_stageDeviation);
		return Particle(emitType, spawnPos, velocity, lifeLength, stageCycles, GenerateRotation(), scale, m_gravityEffect);
	}

	float ParticleSystem::GenerateValue(const float &average, const float &errorPercent) const
	{
		float error = Maths::Random(-1.0f, 1.0f) * errorPercent;
		return average + (average * error);
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
