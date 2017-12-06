﻿#include "SpawnSphere.hpp"

#include "../../maths/Maths.hpp"
#include "../../maths/Vector2.hpp"

namespace Flounder
{
	SpawnSphere::SpawnSphere(const float &radius, const Vector3 &heading) :
		ISpawnParticle(),
		m_radius(radius),
		m_spawnPosition(new Vector3())
	{
	}

	SpawnSphere::~SpawnSphere()
	{
		delete m_spawnPosition;
	}

	Vector3 *SpawnSphere::GetBaseSpawnPosition()
	{
		Vector3::RandomUnitVector(m_spawnPosition);

		m_spawnPosition->Scale(m_radius);
		float a = Maths::RandomInRange(0.0f, 1.0f);
		float b = Maths::RandomInRange(0.0f, 1.0f);

		if (a > b)
		{
			const float temp = a;
			a = b;
			b = temp;
		}

		const float randX = b * cos(2.0f * PI * (a / b));
		const float randY = b * sin(2.0f * PI * (a / b));
		const float distance = Vector2(randX, randY).Length();
		m_spawnPosition->Scale(distance);
		return m_spawnPosition;
	}
}
