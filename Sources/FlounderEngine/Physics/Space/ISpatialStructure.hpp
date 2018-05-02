#pragma once

#include <vector>
#include "Physics/Frustum.hpp"

namespace fl
{
	class GameObject;

	class Component;

	class Collider;

	/// <summary>
	/// A data structure that stores objects with a notion of flounder.space.
	/// </summary>
	template<typename T>
	class FL_EXPORT ISpatialStructure
	{
	public:
		/// <summary>
		/// Creates a new spatial structure.
		/// </summary>
		ISpatialStructure()
		{
		}

		/// <summary>
		/// Deconstructor for the spatial structure.
		/// </summary>
		virtual ~ISpatialStructure()
		{
		}

		/// <summary>
		/// Adds a new object to the spatial structure.
		/// </summary>
		/// <param name="object"> The object to add. </param>
		virtual void Add(T object) = 0;

		/// <summary>
		/// Removes an object from the spatial structure.
		/// </summary>
		/// <param name="object"> The object to remove. </param>
		virtual void Remove(T object) = 0;

		/// <summary>
		/// Removes all objects from the spatial structure..
		/// </summary>
		virtual void Clear() = 0;

		/// <summary>
		/// Gets the size of this structure.
		/// </summary>
		/// <returns> The structures size. </returns>
		virtual unsigned int GetSize() = 0;

		/// <summary>
		/// Returns a set of all objects in the spatial structure.
		/// </summary>
		/// </param>
		/// <returns> The list specified by of all objects. </returns>
		virtual std::vector<T> *GetAll() = 0;

		/// <summary>
		/// Returns a set of all objects in the spatial structure.
		/// </summary>
		/// <param name="result"> The list to store the data into.
		/// </param>
		/// <returns> The list specified by of all objects. </returns>
		virtual std::vector<T> *QueryAll(std::vector<T> *result) = 0;

		/// <summary>
		/// Returns a set of all objects in a specific range of the spatial structure.
		/// </summary>
		/// <param name="range"> The frustum range of space being queried. </param>
		/// <param name="result"> The list to store the data into.
		/// </param>
		/// <returns> The list of all object in range. </returns>
		virtual std::vector<T> *QueryFrustum(Frustum *range, std::vector<T> *result) = 0;

		/// <summary>
		/// Returns a set of all objects in a specific range of the spatial structure.
		/// </summary>
		/// <param name="range"> The shape range of space being queried. </param>
		/// <param name="result"> The list to store the data into.
		/// </param>
		/// <returns> The list of all object in range. </returns>
		virtual std::vector<T> *QueryBounding(Collider *range, std::vector<T> *result) = 0;

		/// <summary>
		/// If the structure contains the object.
		/// </summary>
		/// <param name="object"> The object to check for.
		/// </param>
		/// <returns> If the structure contains the object. </returns>
		virtual bool Contains(GameObject *object) = 0;
	};
}
