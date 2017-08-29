#pragma once

#include "../../maths/vector3.hpp"

namespace flounder
{
	/// <summary>
	/// A interface that defines a particle spawn type.
	/// </summary>
	class ispawnparticle
	{
	public:
		/// <summary>
		/// Creates a new particle spawn.
		/// </summary>
		ispawnparticle()
		{
		}

		/// <summary>
		/// Deconstructor for the particle spawn.
		/// </summary>
		virtual ~ispawnparticle()
		{
		}

		/// <summary>
		/// Gets the base spawn position.
		/// </summary>
		/// <returns> The base spawn position. </returns>
		virtual vector3 *getBaseSpawnPosition() = 0;
	};
}
