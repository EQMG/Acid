#include "ParticleSystem.hpp"

#include "Maths/Maths.hpp"
#include "Particles.hpp"
#include "Scenes/Entity.hpp"

namespace acid
{
ParticleSystem::ParticleSystem(std::vector<std::shared_ptr<ParticleType>> types, const float& pps, const float& averageSpeed, const float& gravityEffect)
	: m_types(std::move(types)), m_pps(pps), m_averageSpeed(averageSpeed), m_gravityEffect(gravityEffect), m_randomRotation(false), m_directionDeviation(0.0f), m_speedDeviation(0.0f), m_lifeDeviation(0.0f), m_stageDeviation(0.0f), m_scaleDeviation(0.0f),
	  m_emitTimer(Time::Seconds(1.0f / m_pps))
{
}

void ParticleSystem::Start() {}

void ParticleSystem::Update()
{
	if(m_types.empty())
		{
			return;
		}

	if(m_emitTimer.IsPassedTime())
		{
			float pastFactor = std::floor(m_emitTimer.GetDifference() / m_emitTimer.GetInterval());
			m_emitTimer.ResetStartTime();

			auto emitters = GetParent()->GetComponents<Emitter>();

			if(emitters.empty())
				{
					return;
				}

			for(uint32_t i = 0; i < pastFactor; i++)
				{
					Particles::Get()->AddParticle(EmitParticle(*emitters[static_cast<uint32_t>(Maths::Random(0, emitters.size()))]));
				}
		}
}

void ParticleSystem::Decode(const Metadata& metadata)
{
	auto typesNode = metadata.FindChild("Types");

	if(typesNode != nullptr)
		{
			for(const auto& typeNode : typesNode->GetChildren())
				{
					m_types.emplace_back(ParticleType::Create(*typeNode));
				}
		}

	metadata.GetChild("PPS", m_pps);
	metadata.GetChild("Average Speed", m_averageSpeed);
	metadata.GetChild("Gravity Effect", m_gravityEffect);
	metadata.GetChild("Random Rotation", m_randomRotation);
	metadata.GetChild("Direction", m_direction);
	metadata.GetChild("Direction Deviation", m_directionDeviation);
	metadata.GetChild("Speed Deviation", m_speedDeviation);
	metadata.GetChild("Life Deviation", m_lifeDeviation);
	metadata.GetChild("Stage Deviation", m_stageDeviation);
	metadata.GetChild("Scale Deviation", m_scaleDeviation);
	m_emitTimer = Timer(Time::Seconds(1.0f / m_pps));
}

void ParticleSystem::Encode(Metadata& metadata) const
{
	auto typesNode = metadata.FindChild("Types", false);

	if(typesNode == nullptr)
		{
			typesNode = metadata.AddChild(new Metadata("Types"));
		}

	for(const auto& type : m_types)
		{
			type->Encode(*typesNode->AddChild(new Metadata()));
		}

	metadata.SetChild("PPS", m_pps);
	metadata.SetChild("Average Speed", m_averageSpeed);
	metadata.SetChild("Gravity Effect", m_gravityEffect);
	metadata.SetChild("Random Rotation", m_randomRotation);
	metadata.SetChild("Direction", m_direction);
	metadata.SetChild("Direction Deviation", m_directionDeviation);
	metadata.SetChild("Speed Deviation", m_speedDeviation);
	metadata.SetChild("Life Deviation", m_lifeDeviation);
	metadata.SetChild("Stage Deviation", m_stageDeviation);
	metadata.SetChild("Scale Deviation", m_scaleDeviation);
}

void ParticleSystem::AddParticleType(const std::shared_ptr<ParticleType>& type)
{
	if(std::find(m_types.begin(), m_types.end(), type) != m_types.end())
		{
			return;
		}

	m_types.emplace_back(type);
}

bool ParticleSystem::RemoveParticleType(const std::shared_ptr<ParticleType>& type)
{
	for(auto it = m_types.begin(); it != m_types.end(); ++it)
		{
			if(*it == type)
				{
					m_types.erase(it);
					return true;
				}
		}

	return false;
}

void ParticleSystem::SetPps(const float& pps)
{
	m_pps = pps;
	m_emitTimer = Timer(Time::Seconds(1.0f / m_pps));
}

void ParticleSystem::SetDirection(const Vector3& direction, const float& deviation)
{
	m_direction = direction;
	m_directionDeviation = deviation * Maths::Pi;
}

Particle ParticleSystem::EmitParticle(const Emitter& emitter)
{
	auto worldTransform = GetParent()->GetWorldTransform() * emitter.GetLocalTransform();
	Vector3 spawnPos = emitter.GeneratePosition() + worldTransform.GetPosition();

	Vector3 velocity;

	if(m_direction != 0.0f)
		{
			velocity = Vector3::RandomUnitVectorWithinCone(m_direction, m_directionDeviation);
		}
	else
		{
			velocity = GenerateRandomUnitVector();
		}

	velocity = velocity.Normalize();
	velocity *= GenerateValue(m_averageSpeed, m_speedDeviation);

	auto emitType = m_types.at(static_cast<uint32_t>(std::floor(Maths::Random(0, m_types.size()))));
	float scale = GenerateValue(emitType->GetScale(), m_scaleDeviation);
	float lifeLength = GenerateValue(emitType->GetLifeLength(), m_lifeDeviation);
	float stageCycles = GenerateValue(emitType->GetStageCycles(), m_stageDeviation);
	return Particle(emitType, spawnPos, velocity, lifeLength, stageCycles, GenerateRotation(), scale, m_gravityEffect);
}

float ParticleSystem::GenerateValue(const float& average, const float& errorPercent) const
{
	float error = Maths::Random(-1.0f, 1.0f) * errorPercent;
	return average + (average * error);
}

float ParticleSystem::GenerateRotation() const
{
	if(m_randomRotation)
		{
			return Maths::Random(0.0f, 360.0f);
		}

	return 0.0f;
}

Vector3 ParticleSystem::GenerateRandomUnitVector() const
{
	float theta = Maths::Random(0.0f, 1.0f) * 2.0f * Maths::Pi;
	float z = Maths::Random(0.0f, 1.0f) * 2.0f - 1.0f;
	float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
	float x = rootOneMinusZSquared * std::cos(theta);
	float y = rootOneMinusZSquared * std::sin(theta);
	return Vector3(x, y, z);
}
}
