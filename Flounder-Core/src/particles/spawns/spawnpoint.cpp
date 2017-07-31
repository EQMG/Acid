#include "spawnpoint.h"

namespace flounder
{
	spawnpoint::spawnpoint() :
		ispawnparticle()
	{
		m_point = new vector3();
	}

	spawnpoint::~spawnpoint()
	{
		delete m_point;
	}

	vector3 *spawnpoint::getBaseSpawnPosition()
	{
		return m_point;
	}
}
