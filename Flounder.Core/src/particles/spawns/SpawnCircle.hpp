#pragma once

#include "ispawnparticle.hpp"

namespace Flounder
{
	class spawncircle :
		public ispawnparticle
	{
	private:
		float m_radius;
		Vector3 *m_heading;
		Vector3 *m_spawnPosition;
	public:
		spawncircle(const float &radius, const Vector3 &heading);

		~spawncircle();

		float getRadius() { return m_radius; }

		void setRadius(const float &radius) { m_radius = radius; }

		Vector3 *getHeading() { return m_heading; }

		void setHeading(const Vector3 &heading) { m_heading->set(heading); }

		Vector3 *getBaseSpawnPosition() override;
	};
}
