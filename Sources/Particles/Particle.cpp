#include "Particle.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
static const float FADE_TIME = 1.0f;

Particle::Particle(std::shared_ptr<ParticleType> particleType, const Vector3& position, const Vector3& velocity, const float& lifeLength, const float& stageCycles, const float& rotation, const float& scale, const float& gravityEffect)
	: m_particleType(std::move(particleType)), m_position(position), m_velocity(velocity), m_lifeLength(lifeLength), m_stageCycles(stageCycles), m_rotation(rotation), m_scale(scale), m_gravityEffect(gravityEffect), m_elapsedTime(0.0f), m_transparency(1.0f),
	  m_textureBlendFactor(0.0f), m_distanceToCamera(0.0f)
{
}

void Particle::Update()
{
	float delta = Engine::Get()->GetDelta().AsSeconds();

	m_velocity.m_y += -10.0f * m_gravityEffect * delta;
	m_change = m_velocity;
	m_change *= delta;

	m_position += m_change;
	m_elapsedTime += delta;

	if(m_elapsedTime > m_lifeLength - FADE_TIME)
		{
			m_transparency -= delta / FADE_TIME;
		}

	if(!IsAlive() || Scenes::Get()->GetCamera() == nullptr)
		{
			return;
		}

	Vector3 cameraToParticle = Scenes::Get()->GetCamera()->GetPosition() - m_position;
	m_distanceToCamera = cameraToParticle.LengthSquared();

	float lifeFactor = m_stageCycles * m_elapsedTime / m_lifeLength;

	if(m_particleType->GetTexture() == nullptr)
		{
			return;
		}

	auto stageCount = static_cast<int32_t>(pow(m_particleType->GetNumberOfRows(), 2));
	float atlasProgression = lifeFactor * stageCount;
	auto index1 = static_cast<int32_t>(std::floor(atlasProgression));
	int32_t index2 = index1 < stageCount - 1 ? index1 + 1 : index1;

	m_textureBlendFactor = std::fmod(atlasProgression, 1.0f);
	m_textureOffset1 = CalculateTextureOffset(index1);
	m_textureOffset2 = CalculateTextureOffset(index2);
}

bool Particle::operator<(const Particle& other) const
{
	return m_distanceToCamera > other.m_distanceToCamera;
}

Vector2 Particle::CalculateTextureOffset(const int32_t& index) const
{
	int32_t column = index % m_particleType->GetNumberOfRows();
	int32_t row = index / m_particleType->GetNumberOfRows();
	Vector2 result = Vector2();
	result.m_x = static_cast<float>(column) / m_particleType->GetNumberOfRows();
	result.m_y = static_cast<float>(row) / m_particleType->GetNumberOfRows();
	return result;
}
}
