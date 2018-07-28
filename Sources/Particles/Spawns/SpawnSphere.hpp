#pragma once

#include "ISpawnParticle.hpp"

namespace acid
{
	class ACID_EXPORT SpawnSphere :
		public ISpawnParticle
	{
	private:
		float m_radius;
		Vector3 m_spawnPosition;
	public:
		SpawnSphere(const float &radius, const Vector3 &heading);

		~SpawnSphere();

		Vector3 GetBaseSpawnPosition() override;

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }
	};
}
