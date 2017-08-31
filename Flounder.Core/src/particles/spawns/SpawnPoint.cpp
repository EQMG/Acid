#include "spawnpoint.hpp"

namespace Flounder
{
	spawnpoint::spawnpoint() :
		ispawnparticle(),
		m_point(new Vector3())
	{
	}

	spawnpoint::~spawnpoint()
	{
		delete m_point;
	}

	Vector3 *spawnpoint::getBaseSpawnPosition()
	{
		return m_point;
	}
}
