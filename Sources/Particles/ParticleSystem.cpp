#include "ParticleSystem.hpp"

#include "Maths/Maths.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/Entity.hpp"
#include "Particles.hpp"

namespace acid {
ParticleSystem::ParticleSystem(std::vector<std::shared_ptr<ParticleType>> types, float pps, float averageSpeed, float gravityEffect) :
	m_types(std::move(types)),
	m_pps(pps),
	m_averageSpeed(averageSpeed),
	m_gravityEffect(gravityEffect),
	m_randomRotation(false),
	m_elapsedEmit(Time::Seconds(1.0f / m_pps)) {
}

void ParticleSystem::Start() {
}

void ParticleSystem::Update() {
	if (m_types.empty()) {
		return;
	}

	m_elapsedEmit.SetInterval(Time::Seconds(1.0f / m_pps));

	if (auto elapsed = m_elapsedEmit.GetElapsed(); elapsed) {
		auto emitters = GetEntity()->GetComponents<Emitter>();

		if (!emitters.empty()) {
			for (uint32_t i = 0; i < elapsed; i++) {
				Particles::Get()->AddParticle(EmitParticle(*emitters[static_cast<uint32_t>(Maths::Random(0.0f, static_cast<float>(emitters.size())))]));
			}
		}
	}
}

void ParticleSystem::AddParticleType(const std::shared_ptr<ParticleType> &type) {
	if (std::find(m_types.begin(), m_types.end(), type) != m_types.end()) {
		return;
	}

	m_types.emplace_back(type);
}

bool ParticleSystem::RemoveParticleType(const std::shared_ptr<ParticleType> &type) {
	for (auto it = m_types.begin(); it != m_types.end(); ++it) {
		if (*it == type) {
			m_types.erase(it);
			return true;
		}
	}

	return false;
}

Vector3f ParticleSystem::RandomUnitVectorWithinCone(const Vector3f &coneDirection, float angle) const {
	auto cosAngle = std::cos(angle);
	auto theta = Maths::Random(0.0f, 1.0f) * 2.0f * Maths::Pi<float>;
	auto z = (cosAngle + Maths::Random(0.0f, 1.0f)) * (1.0f - cosAngle);
	auto rootOneMinusZSquared = std::sqrt(1.0f - z * z);
	auto x = rootOneMinusZSquared * std::cos(theta);
	auto y = rootOneMinusZSquared * std::sin(theta);

	Vector4f direction(x, y, z, 1.0f);

	if (coneDirection.m_x != 0.0f || coneDirection.m_y != 0.0f || (coneDirection.m_z != 1.0f && coneDirection.m_z != -1.0f)) {
		auto rotateAxis = coneDirection.Cross(Vector3f::Front);
		rotateAxis.Normalize();
		auto rotateAngle = std::acos(coneDirection.Dot(Vector3f::Front));

		Matrix4 rotationMatrix;
		rotationMatrix = rotationMatrix.Rotate(-rotateAngle, rotateAxis);
		direction = rotationMatrix.Transform(direction);
	} else if (coneDirection.m_z == -1.0f) {
		direction.m_z *= -1.0f;
	}

	return {direction};
}

void ParticleSystem::SetPps(float pps) {
	m_pps = pps;
}

void ParticleSystem::SetDirection(const Vector3f &direction, float deviation) {
	m_direction = direction;
	m_directionDeviation = deviation * Maths::Pi<float>;
}

Particle ParticleSystem::EmitParticle(const Emitter &emitter) {
	auto spawnPos = emitter.GeneratePosition();

	if (auto transform = GetEntity()->GetComponent<Transform>(); transform) {
		spawnPos += transform->GetPosition();
	}

	Vector3f velocity;

	if (m_direction != Vector3f::Zero) {
		velocity = RandomUnitVectorWithinCone(m_direction, m_directionDeviation);
	} else {
		velocity = GenerateRandomUnitVector();
	}

	velocity = velocity.Normalize();
	velocity *= GenerateValue(m_averageSpeed, m_speedDeviation);

	auto emitType = m_types.at(static_cast<uint32_t>(std::floor(Maths::Random(0.0f, static_cast<float>(m_types.size())))));
	auto scale = GenerateValue(emitType->GetScale(), m_scaleDeviation);
	auto lifeLength = GenerateValue(emitType->GetLifeLength(), m_lifeDeviation);
	auto stageCycles = GenerateValue(emitType->GetStageCycles(), m_stageDeviation);
	return {emitType, spawnPos, velocity, lifeLength, stageCycles, GenerateRotation(), scale, m_gravityEffect};
}

float ParticleSystem::GenerateValue(float average, float errorPercent) {
	auto error = Maths::Random(-1.0f, 1.0f) * errorPercent;
	return average + (average * error);
}

float ParticleSystem::GenerateRotation() const {
	if (m_randomRotation) {
		return Maths::Random(0.0f, Maths::Pi<float>);
	}

	return 0.0f;
}

Vector3f ParticleSystem::GenerateRandomUnitVector() const {
	auto theta = Maths::Random(0.0f, 1.0f) * 2.0f * Maths::Pi<float>;
	auto z = Maths::Random(0.0f, 1.0f) * 2.0f - 1.0f;
	auto rootOneMinusZSquared = std::sqrt(1.0f - z * z);
	auto x = rootOneMinusZSquared * std::cos(theta);
	auto y = rootOneMinusZSquared * std::sin(theta);
	return {x, y, z};
}

const Node &operator>>(const Node &node, ParticleSystem &particleSystem) {
	node["types"].Get(particleSystem.m_types);
	node["pps"].Get(particleSystem.m_pps);
	node["averageSpeed"].Get(particleSystem.m_averageSpeed);
	node["gravityEffect"].Get(particleSystem.m_gravityEffect);
	node["randomRotation"].Get(particleSystem.m_randomRotation);
	node["direction"].Get(particleSystem.m_direction);
	node["directionDeviation"].Get(particleSystem.m_directionDeviation);
	node["speedDeviation"].Get(particleSystem.m_speedDeviation);
	node["lifeDeviation"].Get(particleSystem.m_lifeDeviation);
	node["stageDeviation"].Get(particleSystem.m_stageDeviation);
	node["scaleDeviation"].Get(particleSystem.m_scaleDeviation);
	return node;
}

Node &operator<<(Node &node, const ParticleSystem &particleSystem) {
	node["types"].Set(particleSystem.m_types);
	node["pps"].Set(particleSystem.m_pps);
	node["averageSpeed"].Set(particleSystem.m_averageSpeed);
	node["gravityEffect"].Set(particleSystem.m_gravityEffect);
	node["randomRotation"].Set(particleSystem.m_randomRotation);
	node["direction"].Set(particleSystem.m_direction);
	node["directionDeviation"].Set(particleSystem.m_directionDeviation);
	node["speedDeviation"].Set(particleSystem.m_speedDeviation);
	node["lifeDeviation"].Set(particleSystem.m_lifeDeviation);
	node["stageDeviation"].Set(particleSystem.m_stageDeviation);
	node["scaleDeviation"].Set(particleSystem.m_scaleDeviation);
	return node;
}
}
