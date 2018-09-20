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
		/// Adds a new object to the spatial structure.
		/// </summary>
		/// <param name="object"> The object to add. </param>
		virtual void Add(std::unique_ptr<GameObject> object) = 0;

		/// <summary>
		/// Removes an object from the spatial structure.
		/// </summary>
		/// <param name="object"> The object to remove. </param>
		/// <returns> If the object was removed. </returns>
		virtual bool Remove(GameObject *object) = 0;

		/// <summary>
		/// Moves an object to another spatial structure.
		/// </summary>
		/// <param name="object"> The object to remove. </param>
		/// <param name="structure"> The structure to move to. </param>
		/// <returns> If the object was moved. </returns>
		virtual bool Move(GameObject *object, ISpatialStructure *structure) = 0;

		/// <summary>
		/// Removes all objects from the spatial structure..
		/// </summary>
		virtual void Clear() = 0;

		/// <summary>
		/// Updates all of the game object.
		/// </summary>
		virtual void Update() = 0;

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
		virtual std::vector<GameObject *> QueryAll() = 0;

		/// <summary>
		/// Returns a set of all objects in a spatial objects contained in a frustum.
		/// </summary>
		/// <param name="range"> The frustum range of space being queried. </param>
		/// </param>
		/// <returns> The list of all object in range. </returns>
		virtual std::vector<GameObject *> QueryFrustum(const Frustum &range) = 0;

		/// <summary>
		/// Returns a set of all objects in a spatial objects contained in a sphere.
		/// </summary>
		/// <param name="centre"> The centre of the sphere. </param>
		/// <param name="radius"> The spheres radius. </param>
		/// <param name="result"> The list to store the data into.
		/// </param>
		/// <returns> The list of all object in range. </returns>
	//	virtual std::vector<GameObject *> QuerySphere(const Vector3 &centre, const Vector3 &radius) = 0;

		/// <summary>
		/// Returns a set of all objects in a spatial objects contained in a cube.
		/// </summary>
		/// <param name="min"> The minimum point of the cube. </param>
		/// <param name="max"> The maximum point of the cube. </param>
		/// <param name="result"> The list to store the data into.
		/// </param>
		/// <returns> The list of all object in range. </returns>
	//	virtual std::vector<GameObject *> QueryCube(const Vector3 &min, const Vector3 &max) = 0;

		/// <summary>
		/// If the structure contains the object.
		/// </summary>
		/// <param name="object"> The object to check for.
		/// </param>
		/// <returns> If the structure contains the object. </returns>
		virtual bool Contains(GameObject *object) = 0;
	};
}
