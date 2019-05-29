﻿#include "ParticleSystem.hpp"

#include "Maths/Maths.hpp"
#include "Scenes/Entity.hpp"
#include "Particles.hpp"

namespace acid
{
ParticleSystem::ParticleSystem(std::vector<std::shared_ptr<ParticleType>> types, const float &pps, const float &averageSpeed, const float &gravityEffect) :
	m_types(std::move(types)),
	m_pps(pps),
	m_averageSpeed(averageSpeed),
	m_gravityEffect(gravityEffect),
	m_randomRotation(false),
	m_directionDeviation(0.0f),
	m_speedDeviation(0.0f),
	m_lifeDeviation(0.0f),
	m_stageDeviation(0.0f),
	m_scaleDeviation(0.0f),
	m_elapsedEmit(Time::Seconds(1.0f / m_pps))
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

	m_elapsedEmit.SetInterval(Time::Seconds(1.0f / m_pps));

	if (auto elapsed = m_elapsedEmit.GetElapsed(); elapsed)
	{
		auto emitters = GetParent()->GetComponents<Emitter>();

		if (!emitters.empty())
		{
			for (uint32_t i = 0; i < elapsed; i++)
			{
				Particles::Get()->AddParticle(EmitParticle(*emitters[static_cast<uint32_t>(Maths::Random(0.0f, static_cast<float>(emitters.size())))]));
			}
		}
	}
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

Vector3f ParticleSystem::RandomUnitVectorWithinCone(const Vector3f &coneDirection, const float &angle) const
{
	auto cosAngle = std::cos(angle);
	auto theta = Maths::Random(0.0f, 1.0f) * 2.0f * Maths::Pi;
	auto z = (cosAngle + Maths::Random(0.0f, 1.0f)) * (1.0f - cosAngle);
	auto rootOneMinusZSquared = std::sqrt(1.0f - z * z);
	auto x = rootOneMinusZSquared * std::cos(theta);
	auto y = rootOneMinusZSquared * std::sin(theta);

	auto direction = Vector4f(x, y, z, 1.0f);

	if (coneDirection.m_x != 0.0f || coneDirection.m_y != 0.0f || (coneDirection.m_z != 1.0f && coneDirection.m_z != -1.0f))
	{
		auto rotateAxis = coneDirection.Cross(Vector3f::Front);
		rotateAxis.Normalize();
		auto rotateAngle = std::acos(coneDirection.Dot(Vector3f::Front));
		auto rotationMatrix = Matrix4();
		rotationMatrix = rotationMatrix.Rotate(-rotateAngle, rotateAxis);
		direction = rotationMatrix.Transform(direction);
	}
	else if (coneDirection.m_z == -1.0f)
	{
		direction.m_z *= -1.0f;
	}

	return Vector3f(direction);
}

void ParticleSystem::SetPps(const float &pps)
{
	m_pps = pps;
}

void ParticleSystem::SetDirection(const Vector3f &direction, const float &deviation)
{
	m_direction = direction;
	m_directionDeviation = deviation * Maths::Pi;
}

Particle ParticleSystem::EmitParticle(const Emitter &emitter)
{
	Vector3f spawnPos = emitter.GeneratePosition() + GetParent()->GetWorldTransform().GetPosition();

	Vector3f velocity;

	if (m_direction != Vector3f())
	{
		velocity = RandomUnitVectorWithinCone(m_direction, m_directionDeviation);
	}
	else
	{
		velocity = GenerateRandomUnitVector();
	}

	velocity = velocity.Normalize();
	velocity *= GenerateValue(m_averageSpeed, m_speedDeviation);

	auto emitType = m_types.at(static_cast<uint32_t>(std::floor(Maths::Random(0.0f, static_cast<float>(m_types.size())))));
	auto scale = GenerateValue(emitType->GetScale(), m_scaleDeviation);
	auto lifeLength = GenerateValue(emitType->GetLifeLength(), m_lifeDeviation);
	auto stageCycles = GenerateValue(emitType->GetStageCycles(), m_stageDeviation);
	return Particle(emitType, spawnPos, velocity, lifeLength, stageCycles, GenerateRotation(), scale, m_gravityEffect);
}

float ParticleSystem::GenerateValue(const float &average, const float &errorPercent)
{
	auto error = Maths::Random(-1.0f, 1.0f) * errorPercent;
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

Vector3f ParticleSystem::GenerateRandomUnitVector() const
{
	float theta = Maths::Random(0.0f, 1.0f) * 2.0f * Maths::Pi;
	float z = Maths::Random(0.0f, 1.0f) * 2.0f - 1.0f;
	float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
	float x = rootOneMinusZSquared * std::cos(theta);
	float y = rootOneMinusZSquared * std::sin(theta);
	return Vector3f(x, y, z);
}

const Metadata &operator>>(const Metadata &metadata, ParticleSystem &particleSystem)
{
	auto typesNode = metadata.FindChild("Types");

	particleSystem.m_types.clear();

	if (typesNode != nullptr)
	{
		for (const auto &typeNode : typesNode->GetChildren())
		{
			particleSystem.m_types.emplace_back(ParticleType::Create(*typeNode));
		}
	}

	metadata.GetChild("PPS", particleSystem.m_pps);
	metadata.GetChild("Average Speed", particleSystem.m_averageSpeed);
	metadata.GetChild("Gravity Effect", particleSystem.m_gravityEffect);
	metadata.GetChild("Random Rotation", particleSystem.m_randomRotation);
	metadata.GetChild("Direction", particleSystem.m_direction);
	metadata.GetChild("Direction Deviation", particleSystem.m_directionDeviation);
	metadata.GetChild("Speed Deviation", particleSystem.m_speedDeviation);
	metadata.GetChild("Life Deviation", particleSystem.m_lifeDeviation);
	metadata.GetChild("Stage Deviation", particleSystem.m_stageDeviation);
	metadata.GetChild("Scale Deviation", particleSystem.m_scaleDeviation);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ParticleSystem &particleSystem)
{
	auto typesNode = metadata.FindChild("Types", false);

	if (typesNode == nullptr)
	{
		typesNode = metadata.AddChild(new Metadata("Types"));
	}

	for (const auto &type : particleSystem.m_types)
	{
		*typesNode->AddChild(new Metadata()) << type;
	}

	metadata.SetChild("PPS", particleSystem.m_pps);
	metadata.SetChild("Average Speed", particleSystem.m_averageSpeed);
	metadata.SetChild("Gravity Effect", particleSystem.m_gravityEffect);
	metadata.SetChild("Random Rotation", particleSystem.m_randomRotation);
	metadata.SetChild("Direction", particleSystem.m_direction);
	metadata.SetChild("Direction Deviation", particleSystem.m_directionDeviation);
	metadata.SetChild("Speed Deviation", particleSystem.m_speedDeviation);
	metadata.SetChild("Life Deviation", particleSystem.m_lifeDeviation);
	metadata.SetChild("Stage Deviation", particleSystem.m_stageDeviation);
	metadata.SetChild("Scale Deviation", particleSystem.m_scaleDeviation);
	return metadata;
}
}
