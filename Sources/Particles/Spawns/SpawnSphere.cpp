#include "SpawnSphere.hpp"

#include "Maths/Maths.hpp"
#include "Maths/Vector2.hpp"

namespace acid
{
	SpawnSphere::SpawnSphere(const float &radius) :
		ISpawnParticle(),
		m_radius(radius),
		m_spawnPosition(Vector3())
	{
	}

	SpawnSphere::~SpawnSphere()
	{
	}

	void SpawnSphere::Decode(const Node &node)
	{
		m_radius = node.GetChild<float>("Radius");
	}

	void SpawnSphere::Encode(Node &node) const
	{
		node.SetChild<std::string>("Type", "SpawnSphere");
		node.SetChild<float>("Radius", m_radius);
	}

	Vector3 SpawnSphere::GetBaseSpawnPosition()
	{
		m_spawnPosition = Vector3::RandomUnitVector();

		m_spawnPosition *= m_radius;
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
		m_spawnPosition *= distance;
		return m_spawnPosition;
	}
}
