#pragma once

#include "../physics/icollider.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents an object that has some notion of space, and can be stored in a <seealso cref="ispatialstructure"/>.
	/// </summary>
	class ISpatialObject
	{
	private:
	public:
		/// <summary>
		/// Creates a new spatial object.
		/// </summary>
		ISpatialObject()
		{
		}

		/// <summary>
		/// Deconstructor for the spatial object.
		/// </summary>
		virtual ~ISpatialObject()
		{
		}

		/// <summary>
		/// Gets the shape that fully encloses the object.
		/// </summary>
		/// <returns> Returns a shape fully enclosing the object. </returns>
		virtual icollider *GetCollider() = 0;
	};
}
