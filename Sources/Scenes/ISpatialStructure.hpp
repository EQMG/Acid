#pragma once

#include <memory>
#include <vector>
#include "Physics/Frustum.hpp"

namespace acid
{
	class GameObject;

	class IComponent;

	class Collider;

	/// <summary>
	/// A data structure that stores objects with a notion of space.
	/// </summary>
	class ACID_EXPORT ISpatialStructure
	{
	public:
		/// <summary>
		/// Adds a new object to the spatial structure.
		/// </summary>
		/// <param name="object"> The object to add. </param>
		virtual void Add(GameObject *object) = 0;

		/// <summary>
		/// Removes an object from the spatial structure.
		/// </summary>
		/// <param name="object"> The object to remove. </param>
		/// <returns> If the object was removed. </returns>
		virtual bool Remove(GameObject *object) = 0;

		/// <summary>
		/// Removes all objects from the spatial structure..
		/// </summary>
		virtual void Clear() = 0;

		/// <summary>
		/// Gets the size of this structure.
		/// </summary>
		/// <returns> The structures size. </returns>
		virtual uint32_t GetSize() = 0;

		/// <summary>
		/// Returns a set of all objects in the spatial structure.
		/// </summary>
		/// </param>
		/// <returns> The list specified by of all objects. </returns>
		virtual std::vector<std::unique_ptr<GameObject>> &GetAll() = 0;

		/// <summary>
		/// Returns a set of all objects in the spatial structure.
		/// </summary>
		/// </param>
		/// <returns> The list specified by of all objects. </returns>
		virtual std::vector<GameObject *> QueryAll() = 0;

		/// <summary>
		/// Returns a set of all objects in a specific range of the spatial structure.
		/// </summary>
		/// <param name="range"> The frustum range of space being queried. </param>
		/// </param>
		/// <returns> The list of all object in range. </returns>
		virtual std::vector<GameObject *> QueryFrustum(const Frustum &range) = 0;

		/// <summary>
		/// Returns a set of all objects in a specific range of the spatial structure.
		/// </summary>
		/// <param name="range"> The shape range of space being queried. </param>
		/// <param name="result"> The list to store the data into.
		/// </param>
		/// <returns> The list of all object in range. </returns>
	//	virtual std::vector<GameObject *> QueryBounding(Collider *range) = 0;

		/// <summary>
		/// If the structure contains the object.
		/// </summary>
		/// <param name="object"> The object to check for.
		/// </param>
		/// <returns> If the structure contains the object. </returns>
		virtual bool Contains(GameObject *object) = 0;
	};
}
