#include "spawnline.hpp"

namespace flounder
{
	spawnline::spawnline(const float &length, const vector3 &axis) :
		ispawnparticle(),
		m_length(length),
		m_axis(new vector3(axis)),
		m_spawnPosition(new vector3())
	{
		m_axis->normalize();
	}

	spawnline::~spawnline()
	{
		delete m_axis;
		delete m_spawnPosition;
	}

	vector3 *spawnline::getBaseSpawnPosition()
	{
		m_spawnPosition->set(*m_axis);
		m_spawnPosition->scale(m_length);
		m_spawnPosition->scale(maths::randomInRange(-0.5f, 0.5f));
		return m_spawnPosition;
	}
}
