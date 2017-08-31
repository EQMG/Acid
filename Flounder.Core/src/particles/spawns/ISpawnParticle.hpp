#pragma once

#include "../../maths/Vector2.hpp"
#include "../../maths/Vector3.hpp"
#include "../../maths/Vector4.hpp"

namespace Flounder
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
		virtual Vector3 *getBaseSpawnPosition() = 0;
	};
}
