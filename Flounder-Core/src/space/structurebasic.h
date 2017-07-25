#pragma once

#include "ispatialstructure.h"

namespace flounder
{
	/// <summary>
	/// A structure of spatial objects for a 3D space.
	/// </summary>
	template<typename t>
	class structurebasic :
		public ispatialstructure<t>
	{
	private:
		std::vector<t> *m_objects;
	public:
		/// <summary>
		/// Creates a new basic structure.
		/// </summary>
		structurebasic();

		/// <summary>
		/// Deconstructor for the basic structure.
		/// </summary>
		~structurebasic();

		void add(t *object) override;

		void remove(t *object) override;

		void clear() override;

		int getSize() override;

		std::vector<t*> getAll(std::vector<t*> *result) override;

		std::vector<t*> queryInFrustum(frustum *range, std::vector<t*> *result) override;

		std::vector<t*> queryInBounding(icollider *range, std::vector<t*> *result) override;

		bool contains(ispatialobject *object) override;
	};
}
