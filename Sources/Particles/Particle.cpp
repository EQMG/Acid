#include "Particle.hpp"

#include "Scenes/Scenes.hpp"

namespace acid {
static constexpr float FADE_TIME = 1.0f;

Particle::Particle(std::shared_ptr<ParticleType> particleType, const Vector3f &position, const Vector3f &velocity, float lifeLength, float stageCycles,
	float rotation, float scale, float gravityEffect) :
	particleType(std::move(particleType)),
	position(position),
	velocity(velocity),
	lifeLength(lifeLength),
	stageCycles(stageCycles),
	rotation(rotation),
	scale(scale),
	gravityEffect(gravityEffect) {
}

void Particle::Update() {
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	velocity.y += -10.0f * gravityEffect * delta;
	change = velocity;
	change *= delta;

	position += change;
	elapsedTime += delta;

	if (elapsedTime > lifeLength - FADE_TIME)
		transparency -= delta / FADE_TIME;

	if (!IsAlive() || !Scenes::Get()->GetScene()->GetCamera())
		return;

	auto cameraToParticle = Scenes::Get()->GetScene()->GetCamera()->GetPosition() - position;
	distanceToCamera = cameraToParticle.LengthSquared();

	auto lifeFactor = stageCycles * elapsedTime / lifeLength;

	if (!particleType->GetImage())
		return;

	auto stageCount = static_cast<int32_t>(std::pow(particleType->GetNumberOfRows(), 2));
	auto atlasProgression = lifeFactor * stageCount;
	auto index1 = static_cast<int32_t>(std::floor(atlasProgression));
	auto index2 = index1 < stageCount - 1 ? index1 + 1 : index1;

	imageBlendFactor = std::fmod(atlasProgression, 1.0f);
	imageOffset1 = CalculateImageOffset(index1);
	imageOffset2 = CalculateImageOffset(index2);
}

bool Particle::operator<(const Particle &rhs) const {
	return distanceToCamera > rhs.distanceToCamera;
}

Vector2f Particle::CalculateImageOffset(int32_t index) const {
	auto column = index % particleType->GetNumberOfRows();
	auto row = index / particleType->GetNumberOfRows();
	return Vector2f(static_cast<float>(column), static_cast<float>(row)) / particleType->GetNumberOfRows();
}
}
