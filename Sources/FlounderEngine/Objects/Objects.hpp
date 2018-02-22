#pragma once

#include "../Engine/Engine.hpp"
#include "GameObject.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing game objects on engine updates.
	/// </summary>
	class F_EXPORT Objects :
		public IModule
	{
	private:
		ISpatialStructure<GameObject *> *m_structure;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Objects *Get()
		{
			return reinterpret_cast<Objects *>(Engine::Get()->GetModule("objects"));
		}

		/// <summary>
		/// Creates a new Entities module.
		/// </summary>
		Objects();

		/// <summary>
		/// Deconstructor for the Entities module.
		/// </summary>
		~Objects();

		void Update() override;

		ISpatialStructure<GameObject *> *GetStructure() const { return m_structure; }

		void SetStructure(ISpatialStructure<GameObject *> *structure) { m_structure = structure; }
	};
}
