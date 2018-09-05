#pragma once

#include "ISpawnParticle.hpp"

namespace acid
{
	class ACID_EXPORT SpawnCircle :
		public ISpawnParticle
	{
	private:
		float m_radius;
		Vector3 m_heading;
		Vector3 m_spawnPosition;
	public:
		SpawnCircle(const float &radius = 1.0f, const Vector3 &heading = Vector3::UP);

		~SpawnCircle();

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		Vector3 GetBaseSpawnPosition() override;

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		Vector3 GetHeading() const { return m_heading; }

		void SetHeading(const Vector3 &heading) { m_heading = heading; }
	};
}
