#include "Particle.hpp"

#include "../Scenes/Scenes.hpp"
#include "../Maths/Maths.hpp"

namespace Flounder
{
	Particle::Particle(ParticleType *particleType, const Vector3 &position, const Vector3 &velocity, const float &lifeLength, const float &rotation, const float &scale, const float &gravityEffect) :
		GameObject(Transform(position)),
		m_particleType(particleType),
		m_velocity(new Vector3(velocity)),
		m_change(new Vector3()),
		m_textureOffset1(new Vector2()),
		m_textureOffset2(new Vector2()),
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
		delete m_velocity;
		delete m_change;

		delete m_textureOffset1;
		delete m_textureOffset2;
	}

	void Particle::Update()
	{
		m_velocity->m_y += -10.0f * m_gravityEffect * Engine::Get()->GetDelta();
		m_change->Set(*m_velocity);
		m_change->Scale(Engine::Get()->GetDelta());

		Vector3::Add(*m_change, *m_transform->GetPosition(), m_transform->GetPosition());
		m_elapsedTime += Engine::Get()->GetDelta();

		if (m_elapsedTime > m_lifeLength)
		{
			m_transparency += 1.0f * Engine::Get()->GetDelta();
		}

		if (!IsAlive() || Scenes::Get()->GetCamera() == nullptr)
		{
			return;
		}

		Vector3 *cameraToParticle = Vector3::Subtract(*Scenes::Get()->GetCamera()->GetPosition(), *m_transform->GetPosition(), nullptr);
		m_distanceToCamera = cameraToParticle->LengthSquared();
		delete cameraToParticle;

		const float lifeFactor = m_elapsedTime / m_lifeLength;

		if (m_particleType->GetTexture() == nullptr)
		{
			return;
		}

		const int stageCount = static_cast<int>(pow(m_particleType->GetTexture()->GetNumberOfRows(), 2));
		const float atlasProgression = lifeFactor * stageCount;
		const int index1 = static_cast<int>(std::floor(atlasProgression));
		const int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;

		m_textureBlendFactor = std::fmod(atlasProgression, 1.0f);
		UpdateTextureOffset(m_textureOffset1, index1);
		UpdateTextureOffset(m_textureOffset2, index2);
	}

	bool Particle::operator<(const Particle &other) const
	{
		return m_distanceToCamera > other.m_distanceToCamera;
	}

	Vector2 *Particle::UpdateTextureOffset(Vector2 *offset, const int &index) const
	{
		offset->Set(0.0f, 0.0f);
		const int column = index % m_particleType->GetTexture()->GetNumberOfRows();
		const int row = index / m_particleType->GetTexture()->GetNumberOfRows();
		offset->m_x = static_cast<float>(column) / m_particleType->GetTexture()->GetNumberOfRows();
		offset->m_y = static_cast<float>(row) / m_particleType->GetTexture()->GetNumberOfRows();
		return offset;
	}
}
