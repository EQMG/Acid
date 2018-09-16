#pragma once

#include "ISpawnParticle.hpp"

namespace acid
{
	class ACID_EXPORT SpawnPoint :
		public ISpawnParticle
	{
	private:
		Vector3 m_point;
	public:
		SpawnPoint();

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		Vector3 GeneratePosition() override;

		Vector3 GetPoint() const { return m_point; }

		void SetPoint(const Vector3 &point) { m_point = point; }
	};
}
