#pragma once

#include <vector>

#include "../engine/Engine.hpp"

#include "terrain.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing terrains in 3D worlds.
	/// </summary>
	class terrains :
		public IModule
	{
	private:
		std::vector<terrain*> *m_terrains;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static terrains *get()
		{
			return static_cast<terrains*>(Engine::Get()->GetModule("terrains"));
		}

		/// <summary>
		/// Creates a new terrains module.
		/// </summary>
		terrains();

		/// <summary>
		/// Deconstructor for the terrains module.
		/// </summary>
		~terrains();

		void Update() override;

		std::vector<terrain*> *getTerrains() const { return m_terrains; }
	};
}
