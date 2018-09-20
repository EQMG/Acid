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
		explicit SpawnSphere(const float &radius = 1.0f);

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		Vector3 GeneratePosition() override;

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }
	};
}
