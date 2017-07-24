#pragma once

#include "ispawnparticle.h"

namespace flounder
{
	class spawnpoint :
		public ispawnparticle
	{
	private:
		vector3 *m_point;
	public:
		spawnpoint();

		~spawnpoint();

		vector3 *getBaseSpawnPosition() override;
	};
}
