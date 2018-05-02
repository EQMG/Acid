#pragma once

#include "ISpawnParticle.hpp"

namespace fl
{
	class FL_EXPORT SpawnCircle :
		public ISpawnParticle
	{
	private:
		float m_radius;
		Vector3 *m_heading;
		Vector3 *m_spawnPosition;
	public:
		SpawnCircle(const float &radius, const Vector3 &heading);

		~SpawnCircle();

		Vector3 *GetBaseSpawnPosition() override;

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		Vector3 *GetHeading() const { return m_heading; }

		void SetHeading(const Vector3 &heading) { *m_heading = heading; }
	};
}
