#pragma once

#include "ispawnparticle.hpp"

namespace Flounder
{
	class spawnsphere :
		public ispawnparticle
	{
	private:
		float m_radius;
		Vector3 *m_spawnPosition;
	public:
		spawnsphere(const float &radius, const Vector3 &heading);

		~spawnsphere();

		float getRadius() { return m_radius; }

		void setRadius(const float &radius) { m_radius = radius; }

		Vector3 *getBaseSpawnPosition() override;
	};
}
