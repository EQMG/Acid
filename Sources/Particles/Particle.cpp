#include "Particle.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
static const float FADE_TIME{1.0f};

Particle::Particle(std::shared_ptr<ParticleType> particleType, const Vector3f &position, const Vector3f &velocity, float lifeLength, float stageCycles,
	float rotation, float scale, float gravityEffect) :
	m_particleType{std::move(particleType)},
	m_position{position},
	m_velocity{velocity},
	m_lifeLength{lifeLength},
	m_stageCycles{stageCycles},
	m_rotation{rotation},
	m_scale{scale},
	m_gravityEffect{gravityEffect},
	m_elapsedTime{0.0f},
	m_transparency{1.0f},
	m_imageBlendFactor{0.0f},
	m_distanceToCamera{0.0f}
{
}

void Particle::Update()
{
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	m_velocity.m_y += -10.0f * m_gravityEffect * delta;
	m_change = m_velocity;
	m_change *= delta;

	m_position += m_change;
	m_elapsedTime += delta;

	if (m_elapsedTime > m_lifeLength - FADE_TIME)
	{
		m_transparency -= delta / FADE_TIME;
	}

	if (!IsAlive() || Scenes::Get()->GetCamera() == nullptr)
	{
		return;
	}

	auto cameraToParticle = Scenes::Get()->GetCamera()->GetPosition() - m_position;
	m_distanceToCamera = cameraToParticle.LengthSquared();

	auto lifeFactor = m_stageCycles * m_elapsedTime / m_lifeLength;

	if (m_particleType->GetImage() == nullptr)
	{
		return;
	}

	auto stageCount = static_cast<int32_t>(pow(m_particleType->GetNumberOfRows(), 2));
	auto atlasProgression = lifeFactor * stageCount;
	auto index1 = static_cast<int32_t>(std::floor(atlasProgression));
	auto index2 = index1 < stageCount - 1 ? index1 + 1 : index1;

	m_imageBlendFactor = std::fmod(atlasProgression, 1.0f);
	m_imageOffset1 = CalculateImageOffset(index1);
	m_imageOffset2 = CalculateImageOffset(index2);
}

bool Particle::operator<(const Particle &other) const
{
	return m_distanceToCamera > other.m_distanceToCamera;
}

Vector2f Particle::CalculateImageOffset(int32_t index) const
{
	auto column = index % m_particleType->GetNumberOfRows();
	auto row = index / m_particleType->GetNumberOfRows();
	return Vector2f{static_cast<float>(column), static_cast<float>(row)} / m_particleType->GetNumberOfRows();
}
}
