#pragma once

#include "ispawnparticle.hpp"

namespace Flounder
{
	class spawnpoint :
		public ispawnparticle
	{
	private:
		Vector3 *m_point;
	public:
		spawnpoint();

		~spawnpoint();

		Vector3 *getBaseSpawnPosition() override;
	};
}
