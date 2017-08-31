#pragma once

#include "../../maths/maths.hpp"
#include "ispawnparticle.hpp"

namespace Flounder
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

		float getLength() const { return m_length; }

		void setLength(const float &length) { m_length = length; }

		vector3 *getAxis() const { return m_axis; }

		void setAxis(const vector3 &axis) { m_axis->set(axis); }

		vector3 *getSpawnPosition() const { return m_spawnPosition; }

		void setSpawnPosition(const vector3 &spawnPosition) { m_spawnPosition->set(spawnPosition); }
	};
}
