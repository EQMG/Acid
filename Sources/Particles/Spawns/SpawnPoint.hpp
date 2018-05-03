#pragma once

#include "ISpawnParticle.hpp"

namespace fl
{
	class FL_EXPORT SpawnPoint :
		public ISpawnParticle
	{
	private:
		Vector3 *m_point;
	public:
		SpawnPoint();

		~SpawnPoint();

		Vector3 *GetBaseSpawnPosition() override;

		Vector3 *getPoint() const { return m_point; }

		void setPoint(const Vector3 &point) { *m_point = point; }
	};
}
