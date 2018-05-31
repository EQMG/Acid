#include "SpawnSphere.hpp"

#include "Maths/Maths.hpp"
#include "Maths/Vector2.hpp"

namespace fl
{
	SpawnSphere::SpawnSphere(const float &radius, const Vector3 &heading) :
		ISpawnParticle(),
		m_radius(radius),
		m_spawnPosition(Vector3())
	{
	}

	SpawnSphere::~SpawnSphere()
	{
	}

	Vector3 SpawnSphere::GetBaseSpawnPosition()
	{
		m_spawnPosition = Vector3::RandomUnitVector();

		m_spawnPosition *= m_radius;
		float a = Maths::RandomInRange(0.0f, 1.0f);
		float b = Maths::RandomInRange(0.0f, 1.0f);

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
