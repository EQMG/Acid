#pragma once

#include "Maths/Vector3.hpp"

namespace fl
{
	/// <summary>
	/// A interface that defines a particle spawn type.
	/// </summary>
	class FL_EXPORT ISpawnParticle
	{
	public:
		/// <summary>
		/// Creates a new particle spawn.
		/// </summary>
		ISpawnParticle()
		{
		}

		/// <summary>
		/// Deconstructor for the particle spawn.
		/// </summary>
		virtual ~ISpawnParticle()
		{
		}

		/// <summary>
		/// Gets the base spawn position.
		/// </summary>
		/// <returns> The base spawn position. </returns>
		virtual Vector3 *GetBaseSpawnPosition() = 0;
	};
}
