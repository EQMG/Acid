#pragma once

#include <vector>
#include <algorithm>
#include "ISpatialStructure.hpp"

namespace Flounder
{
	/// <summary>
	/// A structure of spatial objects for a 3D space.
	/// </summary>
	template<typename T>
	class StructureBasic :
		public ISpatialStructure<T>
	{
	private:
		std::vector<T> *m_objects;
	public:
		/// <summary>
		/// Creates a new basic structure.
		/// </summary>
		StructureBasic();

		/// <summary>
		/// Deconstructor for the basic structure.
		/// </summary>
		~StructureBasic();

		void Add(T object) override;

		void Remove(T object) override;

		void Clear() override;

		int GetSize() override;

		std::vector<T> *GetAll() override;

		std::vector<T> *QueryAll(std::vector<T> *result) override;

		std::vector<T> *QueryFrustum(Frustum *range, std::vector<T> *result) override;

		std::vector<T> *QueryBounding(ICollider *range, std::vector<T> *result) override;

		bool Contains(ISpatialObject *object) override;
	};
}
