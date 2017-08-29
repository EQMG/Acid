#pragma once

#include "ispawnparticle.hpp"

namespace flounder
{
	class spawnsphere :
		public ispawnparticle
	{
	private:
		float m_radius;
		vector3 *m_spawnPosition;
	public:
		spawnsphere(const float &radius, const vector3 &heading);

		~spawnsphere();

		inline float getRadius() { return m_radius; }

		inline void setRadius(const float &radius) { m_radius = radius; }

		vector3 *getBaseSpawnPosition() override;
	};
}
