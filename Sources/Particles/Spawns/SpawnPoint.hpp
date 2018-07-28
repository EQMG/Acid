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

		~SpawnPoint();

		Vector3 GetBaseSpawnPosition() override;

		Vector3 GetPoint() const { return m_point; }

		void SetPoint(const Vector3 &point) { m_point = point; }
	};
}
