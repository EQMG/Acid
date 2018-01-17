#pragma once

#include "../Engine/Engine.hpp"
#include "Entity.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing entities on engine updates.
	/// </summary>
	class Entities :
		public IModule
	{
	private:
		ISpatialStructure<Entity*> *m_structure;
	//	std::vector<Entity*> *m_entities;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Entities *Get()
		{
			return static_cast<Entities*>(Engine::Get()->GetModule("entities"));
		}

		/// <summary>
		/// Creates a new Entities module.
		/// </summary>
		Entities();

		/// <summary>
		/// Deconstructor for the Entities module.
		/// </summary>
		~Entities();

		void Update() override;

		ISpatialStructure<Entity*> *GetStructure() const { return m_structure; }
	};
}
