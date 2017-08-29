#pragma once

#include "ispawnparticle.hpp"

namespace flounder
{
	class spawncircle :
		public ispawnparticle
	{
	private:
		float m_radius;
		vector3 *m_heading;
		vector3 *m_spawnPosition;
	public:
		spawncircle(const float &radius, const vector3 &heading);

		~spawncircle();

		inline float getRadius() { return m_radius; }

		inline void setRadius(const float &radius) { m_radius = radius; }

		inline vector3 *getHeading() { return m_heading; }

		inline void setHeading(const vector3 &heading) { m_heading->set(heading); }

		vector3 *getBaseSpawnPosition() override;
	};
}
