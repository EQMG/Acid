#pragma once

#include <vector>

#include "../physics/frustum.hpp"

#include "ispatialobject.hpp"

namespace Flounder
{
	/// <summary>
	/// A data structure that stores objects with a notion of flounder.space.
	/// </summary>
	template<class t>
	class ispatialstructure
	{
	public:
		/// <summary>
		/// Creates a new spatial structure.
		/// </summary>
		ispatialstructure()
		{
		}

		/// <summary>
		/// Deconstructor for the spatial structure.
		/// </summary>
		virtual ~ispatialstructure()
		{
		}

		/// <summary>
		/// Adds a new object to the spatial structure.
		/// </summary>
		/// <param name="object"> The object to add. </param>
		virtual void add(t object) = 0;

		/// <summary>
		/// Removes an object from the spatial structure.
		/// </summary>
		/// <param name="object"> The object to remove. </param>
		virtual void remove(t object) = 0;

		/// <summary>
		/// Removes all objects from the spatial structure..
		/// </summary>
		virtual void clear() = 0;

		/// <summary>
		/// Gets the size of this structure.
		/// </summary>
		/// <returns> The structures size. </returns>
		virtual int getSize() = 0;

		/// <summary>
		/// Returns a set of all objects in the spatial structure.
		/// </summary>
		/// </param>
		/// <returns> The list specified by of all objects. </returns>
		virtual std::vector<t> *getAll() = 0;

		/// <summary>
		/// Returns a set of all objects in the spatial structure.
		/// </summary>
		/// <param name="result"> The list to store the data into.
		/// </param>
		/// <returns> The list specified by of all objects. </returns>
		virtual std::vector<t> *getAll(std::vector<t> *result) = 0;

		/// <summary>
		/// Returns a set of all objects in a specific range of the spatial structure.
		/// </summary>
		/// <param name="range"> The frustum range of space being queried. </param>
		/// <param name="result"> The list to store the data into.
		/// </param>
		/// <returns> The list of all object in range. </returns>
		virtual std::vector<t> *queryInFrustum(frustum *range, std::vector<t> *result) = 0;

		/// <summary>
		/// Returns a set of all objects in a specific range of the spatial structure.
		/// </summary>
		/// <param name="range"> The shape range of space being queried. </param>
		/// <param name="result"> The list to store the data into.
		/// </param>
		/// <returns> The list of all object in range. </returns>
		virtual std::vector<t> *queryInBounding(icollider *range, std::vector<t> *result) = 0;

		/// <summary>
		/// If the structure contains the object.
		/// </summary>
		/// <param name="object"> The object to check for.
		/// </param>
		/// <returns> If the structure contains the object. </returns>
		virtual bool contains(ispatialobject *object) = 0;
	};
}
