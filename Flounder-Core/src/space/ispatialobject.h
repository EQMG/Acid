#pragma once

#include "../physics/icollider.h"

namespace flounder
{
	/// <summary>
	/// Represents an object that has some notion of space, and can be stored in a <seealso cref="ispatialstructure"/>.
	/// </summary>
	class ispatialobject
	{
	private:
	public:
		/// <summary>
		/// Creates a new spatial object.
		/// </summary>
		ispatialobject()
		{
		}

		/// <summary>
		/// Deconstructor for the spatial object.
		/// </summary>
		virtual ~ispatialobject()
		{
		}

		/// <summary>
		/// Gets the shape that fully encloses the object.
		/// </summary>
		/// <returns> Returns a shape fully enclosing the object. </returns>
		virtual icollider *getCollider() = 0;
	};
}
