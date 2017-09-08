#pragma once

#include "ISpatialStructure.hpp"

namespace Flounder
{
	/// <summary>
	/// A structure of spatial objects for a 3D space.
	/// </summary>
	template<class t>
	class StructureBasic :
		public ISpatialStructure<t>
	{
	private:
		std::vector<t> *m_objects;
	public:
		/// <summary>
		/// Creates a new basic structure.
		/// </summary>
		StructureBasic();

		/// <summary>
		/// Deconstructor for the basic structure.
		/// </summary>
		~StructureBasic();

		void Add(t object) override;

		void Remove(t object) override;

		void Clear() override;

		int GetSize() override;

		std::vector<t> *GetAll() override;

		std::vector<t> *QueryAll(std::vector<t> *result) override;

		std::vector<t> *QueryFrustum(Frustum *range, std::vector<t> *result) override;

		std::vector<t> *QueryBounding(ICollider *range, std::vector<t> *result) override;

		bool Contains(ISpatialObject *object) override;
	};
}
