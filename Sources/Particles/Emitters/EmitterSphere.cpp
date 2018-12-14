#include "EmitterSphere.hpp"

#include "Maths/Maths.hpp"
#include "Maths/Vector2.hpp"
#include "Objects/Entity.hpp"

namespace acid
{
	EmitterSphere::EmitterSphere(const float &radius, const Transform &localTransform) :
		ParticleEmitter(localTransform),
		m_radius(radius)
	{
	}

	void EmitterSphere::Start()
	{
	}

	void EmitterSphere::Update()
	{
	}

	void EmitterSphere::Decode(const Metadata &metadata)
	{
		m_radius = metadata.GetChild<float>("Radius");
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
	}

	void EmitterSphere::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("Radius", m_radius);
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
	}

	Vector3 EmitterSphere::GeneratePosition() const
	{
		auto spawnPosition = Vector3::RandomUnitVector();

		spawnPosition *= m_radius;
		float a = Maths::Random(0.0f, 1.0f);
		float b = Maths::Random(0.0f, 1.0f);

		if (a > b)
		{
			float temp = a;
			a = b;
			b = temp;
		}

		float randX = b * std::cos(2.0f * PI * (a / b));
		float randY = b * std::sin(2.0f * PI * (a / b));
		float distance = Vector2(randX, randY).Length();
		spawnPosition *= distance;
		return spawnPosition;
	}
}
