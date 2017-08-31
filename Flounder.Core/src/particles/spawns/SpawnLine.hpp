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
		Vector3 *m_axis;
		Vector3 *m_spawnPosition;
	public:
		spawnline(const float &length, const Vector3 &axis);

		~spawnline();

		Vector3 *getBaseSpawnPosition() override;

		float getLength() const { return m_length; }

		void setLength(const float &length) { m_length = length; }

		Vector3 *getAxis() const { return m_axis; }

		void setAxis(const Vector3 &axis) { m_axis->set(axis); }

		Vector3 *getSpawnPosition() const { return m_spawnPosition; }

		void setSpawnPosition(const Vector3 &spawnPosition) { m_spawnPosition->set(spawnPosition); }
	};
}
