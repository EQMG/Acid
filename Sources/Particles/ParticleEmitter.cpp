#include "ParticleEmitter.hpp"

#include "Maths/Maths.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/Entity.inl"
#include "Particles.hpp"

namespace acid {
bool ParticleEmitter::registered = Register("particleEmitter");

ParticleEmitter::ParticleEmitter(std::vector<std::shared_ptr<ParticleType>> types, std::vector<std::unique_ptr<Emitter>> &&emitters,
	float pps, float averageSpeed, float gravityEffect) :
	m_types(std::move(types)),
	m_emitters(std::move(emitters)),
	m_pps(pps),
	m_averageSpeed(averageSpeed),
	m_gravityEffect(gravityEffect),
	m_randomRotation(false),
	m_elapsedEmit(Time::Seconds(1.0f / m_pps)) {
}

void ParticleEmitter::Update() {
	if (m_types.empty()) {
		return;
	}

	m_elapsedEmit.SetInterval(Time::Seconds(1.0f / m_pps));

	if (auto elapsed = m_elapsedEmit.GetElapsed() && !m_emitters.empty()) {
		for (uint32_t i = 0; i < elapsed; i++) {
			Particles::Get()->AddParticle(EmitParticle(m_emitters[static_cast<uint32_t>(Maths::Random(0.0f, 
				static_cast<float>(m_emitters.size())))].get()));
		}
	}
}

void ParticleEmitter::AddParticleType(const std::shared_ptr<ParticleType> &type) {
	if (std::find(m_types.begin(), m_types.end(), type) != m_types.end()) {
		return;
	}

	m_types.emplace_back(type);
}

bool ParticleEmitter::RemoveParticleType(const std::shared_ptr<ParticleType> &type) {
	for (auto it = m_types.begin(); it != m_types.end(); ++it) {
		if (*it == type) {
			m_types.erase(it);
			return true;
		}
	}

	return false;
}

void ParticleEmitter::AddEmitter(std::unique_ptr<Emitter> &&emitter) {
	m_emitters.emplace_back(std::move(emitter));
}

Vector3f ParticleEmitter::RandomUnitVectorWithinCone(const Vector3f &coneDirection, float angle) const {
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

void ParticleEmitter::SetPps(float pps) {
	m_pps = pps;
}

void ParticleEmitter::SetDirection(const Vector3f &direction, float deviation) {
	m_direction = direction;
	m_directionDeviation = deviation * Maths::Pi<float>;
}

Particle ParticleEmitter::EmitParticle(const Emitter *emitter) {
	auto spawnPos = emitter->GeneratePosition();

	if (auto transform = GetEntity()->GetComponent<Transform>()) {
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

float ParticleEmitter::GenerateValue(float average, float errorPercent) {
	auto error = Maths::Random(-1.0f, 1.0f) * errorPercent;
	return average + (average * error);
}

float ParticleEmitter::GenerateRotation() const {
	if (m_randomRotation) {
		return Maths::Random(0.0f, Maths::Pi<float>);
	}

	return 0.0f;
}

Vector3f ParticleEmitter::GenerateRandomUnitVector() const {
	auto theta = Maths::Random(0.0f, 1.0f) * 2.0f * Maths::Pi<float>;
	auto z = Maths::Random(0.0f, 1.0f) * 2.0f - 1.0f;
	auto rootOneMinusZSquared = std::sqrt(1.0f - z * z);
	auto x = rootOneMinusZSquared * std::cos(theta);
	auto y = rootOneMinusZSquared * std::sin(theta);
	return {x, y, z};
}

const Node &operator>>(const Node &node, ParticleEmitter &particleSystem) {
	node["types"].Get(particleSystem.m_types);
	node["emitters"].Get(particleSystem.m_emitters);
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

Node &operator<<(Node &node, const ParticleEmitter &particleSystem) {
	node["types"].Set(particleSystem.m_types);
	node["emitters"].Set(particleSystem.m_emitters);
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
