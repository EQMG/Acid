#pragma once

#include "../Space/StructureBasic.hpp"

namespace Flounder
{
	/// <summary>
	/// A object used to represent a scene.
	/// </summary>
	class F_EXPORT Scene
	{
	private:
		StructureBasic<GameObject *> *m_structure;
	public:
		/// <summary>
		/// Creates a new scene.
		/// </summary>
		Scene();

		/// <summary>
		/// Deconstructor for the scene.
		/// </summary>
		~Scene();

		/// <summary>
		/// Run when updating the scene.
		/// </summary>
		virtual void Update() = 0;

		StructureBasic<GameObject *> *GetStructure() const { return m_structure; }
	};
}
