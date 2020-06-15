#include "ParticleSystem.hpp"

#include "Maths/Maths.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/Entity.hpp"
#include "Particles.hpp"

namespace acid {
ParticleSystem::ParticleSystem(std::vector<std::shared_ptr<ParticleType>> types, std::vector<std::unique_ptr<Emitter>> &&emitters,
	float pps, float averageSpeed, float gravityEffect) :
	types(std::move(types)),
	emitters(std::move(emitters)),
	pps(pps),
	averageSpeed(averageSpeed),
	gravityEffect(gravityEffect),
	randomRotation(false),
	elapsedEmit(Time::Seconds(1.0f / pps)) {
}

void ParticleSystem::Start() {
}

void ParticleSystem::Update() {
	if (types.empty()) return;

	elapsedEmit.SetInterval(Time::Seconds(1.0f / pps));

	if (auto elapsed = elapsedEmit.GetElapsed(); elapsed && !emitters.empty()) {
		for (uint32_t i = 0; i < elapsed; i++) {
			auto emitterIndex = static_cast<uint32_t>(Maths::Random(0.0f, static_cast<float>(emitters.size())));
			Particles::Get()->AddParticle(EmitParticle(emitters[emitterIndex].get()));
		}
	}
}

void ParticleSystem::AddParticleType(const std::shared_ptr<ParticleType> &type) {
	if (std::find(types.begin(), types.end(), type) != types.end())
		return;

	types.emplace_back(type);
}

bool ParticleSystem::RemoveParticleType(const std::shared_ptr<ParticleType> &type) {
	for (auto it = types.begin(); it != types.end(); ++it) {
		if (*it == type) {
			types.erase(it);
			return true;
		}
	}

	return false;
}

void ParticleSystem::AddEmitter(std::unique_ptr<Emitter> &&emitter) {
	emitters.emplace_back(std::move(emitter));
}

Vector3f ParticleSystem::RandomUnitVectorWithinCone(const Vector3f &coneDirection, float angle) const {
	auto cosAngle = std::cos(angle);
	auto theta = Maths::Random(0.0f, 1.0f) * 2.0f * Maths::PI<float>;
	auto z = (cosAngle + Maths::Random(0.0f, 1.0f)) * (1.0f - cosAngle);
	auto rootOneMinusZSquared = std::sqrt(1.0f - z * z);
	auto x = rootOneMinusZSquared * std::cos(theta);
	auto y = rootOneMinusZSquared * std::sin(theta);

	Vector4f direction(x, y, z, 1.0f);

	if (coneDirection.x != 0.0f || coneDirection.y != 0.0f || (coneDirection.z != 1.0f && coneDirection.z != -1.0f)) {
		auto rotateAxis = coneDirection.Cross(Vector3f::Front);
		rotateAxis.Normalize();
		auto rotateAngle = std::acos(coneDirection.Dot(Vector3f::Front));

		Matrix4 rotationMatrix;
		rotationMatrix = rotationMatrix.Rotate(-rotateAngle, rotateAxis);
		direction = rotationMatrix.Transform(direction);
	} else if (coneDirection.z == -1.0f) {
		direction.z *= -1.0f;
	}

	return {direction};
}

void ParticleSystem::SetPps(float pps) {
	this->pps = pps;
}

void ParticleSystem::SetDirection(const Vector3f &direction, float deviation) {
	this->direction = direction;
	directionDeviation = deviation * Maths::PI<float>;
}

Particle ParticleSystem::EmitParticle(const Emitter *emitter) {
	auto spawnPos = emitter->GeneratePosition();

	if (auto transform = GetEntity()->GetComponent<Transform>())
		spawnPos += transform->GetPosition();

	Vector3f velocity;

	if (direction != Vector3f::Zero) {
		velocity = RandomUnitVectorWithinCone(direction, directionDeviation);
	} else {
		velocity = GenerateRandomUnitVector();
	}

	velocity = velocity.Normalize();
	velocity *= GenerateValue(averageSpeed, speedDeviation);

	auto emitType = types.at(static_cast<uint32_t>(std::floor(Maths::Random(0.0f, static_cast<float>(types.size())))));
	auto scale = GenerateValue(emitType->GetScale(), scaleDeviation);
	auto lifeLength = GenerateValue(emitType->GetLifeLength(), lifeDeviation);
	auto stageCycles = GenerateValue(emitType->GetStageCycles(), stageDeviation);
	return {emitType, spawnPos, velocity, lifeLength, stageCycles, GenerateRotation(), scale, gravityEffect};
}

float ParticleSystem::GenerateValue(float average, float errorPercent) {
	auto error = Maths::Random(-1.0f, 1.0f) * errorPercent;
	return average + (average * error);
}

float ParticleSystem::GenerateRotation() const {
	if (randomRotation)
		return Maths::Random(0.0f, Maths::PI<float>);

	return 0.0f;
}

Vector3f ParticleSystem::GenerateRandomUnitVector() const {
	auto theta = Maths::Random(0.0f, 1.0f) * 2.0f * Maths::PI<float>;
	auto z = Maths::Random(0.0f, 1.0f) * 2.0f - 1.0f;
	auto rootOneMinusZSquared = std::sqrt(1.0f - z * z);
	auto x = rootOneMinusZSquared * std::cos(theta);
	auto y = rootOneMinusZSquared * std::sin(theta);
	return {x, y, z};
}

const Node &operator>>(const Node &node, ParticleSystem &particleSystem) {
	node["types"].Get(particleSystem.types);
	node["emitters"].Get(particleSystem.emitters);
	node["pps"].Get(particleSystem.pps);
	node["averageSpeed"].Get(particleSystem.averageSpeed);
	node["gravityEffect"].Get(particleSystem.gravityEffect);
	node["randomRotation"].Get(particleSystem.randomRotation);
	node["direction"].Get(particleSystem.direction);
	node["directionDeviation"].Get(particleSystem.directionDeviation);
	node["speedDeviation"].Get(particleSystem.speedDeviation);
	node["lifeDeviation"].Get(particleSystem.lifeDeviation);
	node["stageDeviation"].Get(particleSystem.stageDeviation);
	node["scaleDeviation"].Get(particleSystem.scaleDeviation);
	return node;
}

Node &operator<<(Node &node, const ParticleSystem &particleSystem) {
	node["types"].Set(particleSystem.types);
	node["emitters"].Set(particleSystem.emitters);
	node["pps"].Set(particleSystem.pps);
	node["averageSpeed"].Set(particleSystem.averageSpeed);
	node["gravityEffect"].Set(particleSystem.gravityEffect);
	node["randomRotation"].Set(particleSystem.randomRotation);
	node["direction"].Set(particleSystem.direction);
	node["directionDeviation"].Set(particleSystem.directionDeviation);
	node["speedDeviation"].Set(particleSystem.speedDeviation);
	node["lifeDeviation"].Set(particleSystem.lifeDeviation);
	node["stageDeviation"].Set(particleSystem.stageDeviation);
	node["scaleDeviation"].Set(particleSystem.scaleDeviation);
	return node;
}
}
