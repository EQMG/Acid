#pragma once

#include "../../maths/Vector2.hpp"
#include "../../maths/Vector3.hpp"
#include "../../maths/Vector4.hpp"

namespace Flounder
{
	/// <summary>
	/// A interface that defines a particle spawn type.
	/// </summary>
	class ISpawnParticle
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
