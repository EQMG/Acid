#pragma once

#include "../../maths/maths.hpp"
#include "ispawnparticle.hpp"

namespace flounder
{
	class spawnline :
		public ispawnparticle
	{
	private:
		float m_length;
		vector3 *m_axis;
		vector3 *m_spawnPosition;
	public:
		spawnline(const float &length, const vector3 &axis);

		~spawnline();

		vector3 *getBaseSpawnPosition() override;

		inline float getLength() const { return m_length; }

		inline void setLength(const float &length) { m_length = length; }

		inline vector3 *getAxis() const { return m_axis; }

		inline void setAxis(const vector3 &axis) { m_axis->set(axis); }

		inline vector3 *getSpawnPosition() const { return m_spawnPosition; }

		inline void setSpawnPosition(const vector3 &spawnPosition) { m_spawnPosition->set(spawnPosition); }
	};
}
