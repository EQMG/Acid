#include "Particle.hpp"

#include "Scenes/Scenes.hpp"
#include "Maths/Maths.hpp"

namespace fl
{
	Particle::Particle(std::shared_ptr<ParticleType> particleType, const Vector3 &position, const Vector3 &velocity, const float &lifeLength, const float &rotation, const float &scale, const float &gravityEffect) :
		m_particleType(particleType),
		m_position(position),
		m_velocity(velocity),
		m_change(Vector3()),
		m_textureOffset1(Vector2()),
		m_textureOffset2(Vector2()),
		m_lifeLength(lifeLength),
		m_rotation(rotation),
		m_scale(scale),
		m_gravityEffect(gravityEffect),
		m_elapsedTime(0.0f),
		m_transparency(0.0f),
		m_textureBlendFactor(0.0f),
		m_distanceToCamera(0.0f)
	{
	}

	Particle::~Particle()
	{
	}

	void Particle::Update()
	{
		m_velocity.m_y += -10.0f * m_gravityEffect * Engine::Get()->GetDelta();
		m_change = m_velocity;
		m_change *= Engine::Get()->GetDelta();

		m_position += m_change;
		m_elapsedTime += Engine::Get()->GetDelta();

		if (m_elapsedTime > m_lifeLength)
		{
			m_transparency += 1.0f * Engine::Get()->GetDelta();
		}

		if (!IsAlive() || Scenes::Get()->GetCamera() == nullptr)
		{
			return;
		}

		Vector3 cameraToParticle = Scenes::Get()->GetCamera()->GetPosition() - m_position;
		m_distanceToCamera = cameraToParticle.LengthSquared();

		float lifeFactor = m_elapsedTime / m_lifeLength;

		if (m_particleType->GetTexture() == nullptr)
		{
			return;
		}

		int stageCount = static_cast<int>(pow(m_particleType->GetTexture()->GetNumberOfRows(), 2));
		float atlasProgression = lifeFactor * stageCount;
		int index1 = static_cast<int>(std::floor(atlasProgression));
		int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;

		m_textureBlendFactor = std::fmod(atlasProgression, 1.0f);
		m_textureOffset1 = CalculateTextureOffset(index1);
		m_textureOffset2 = CalculateTextureOffset(index2);
	}

	bool Particle::operator<(const Particle &other) const
	{
		return m_distanceToCamera > other.m_distanceToCamera;
	}

	Vector2 Particle::CalculateTextureOffset(const int &index) const
	{
		int column = index % m_particleType->GetTexture()->GetNumberOfRows();
		int row = index / m_particleType->GetTexture()->GetNumberOfRows();
		Vector2 result = Vector2();
		result.m_x = static_cast<float>(column) / m_particleType->GetTexture()->GetNumberOfRows();
		result.m_y = static_cast<float>(row) / m_particleType->GetTexture()->GetNumberOfRows();
		return result;
	}
}
