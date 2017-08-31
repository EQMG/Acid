#include "spawnsphere.hpp"

namespace Flounder
{
	spawnsphere::spawnsphere(const float &radius, const Vector3 &heading) :
		ispawnparticle(),
		m_radius(radius),
		m_spawnPosition(new Vector3())
	{
	}

	spawnsphere::~spawnsphere()
	{
		delete m_spawnPosition;
	}

	Vector3 *spawnsphere::getBaseSpawnPosition()
	{
		Vector3::generateRandomUnitVector(m_spawnPosition);

		m_spawnPosition->scale(m_radius);
		float a = Maths::RandomInRange(0.0f, 1.0f);
		float b = Maths::RandomInRange(0.0f, 1.0f);

		if (a > b)
		{
			float temp = a;
			a = b;
			b = temp;
		}

		float randX = b * cos(2.0f * PI * (a / b));
		float randY = b * sin(2.0f * PI * (a / b));
		float distance = Vector2(randX, randY).length();
		m_spawnPosition->scale(distance);
		return m_spawnPosition;
	}
}
