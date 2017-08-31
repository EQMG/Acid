#include "spawnline.hpp"

namespace Flounder
{
	spawnline::spawnline(const float &length, const Vector3 &axis) :
		ispawnparticle(),
		m_length(length),
		m_axis(new Vector3(axis)),
		m_spawnPosition(new Vector3())
	{
		m_axis->normalize();
	}

	spawnline::~spawnline()
	{
		delete m_axis;
		delete m_spawnPosition;
	}

	Vector3 *spawnline::getBaseSpawnPosition()
	{
		m_spawnPosition->set(*m_axis);
		m_spawnPosition->scale(m_length);
		m_spawnPosition->scale(Maths::RandomInRange(-0.5f, 0.5f));
		return m_spawnPosition;
	}
}
