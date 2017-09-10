#pragma once

#include <vector>

#include "../engine/Engine.hpp"

#include "Terrain.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing terrains in 3D worlds.
	/// </summary>
	class Terrains :
		public IModule
	{
	private:
		std::vector<Terrain*> *m_terrains;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Terrains *get()
		{
			return static_cast<Terrains*>(Engine::Get()->GetModule("terrains"));
		}

		/// <summary>
		/// Creates a new terrains module.
		/// </summary>
		Terrains();

		/// <summary>
		/// Deconstructor for the terrains module.
		/// </summary>
		~Terrains();

		void Update() override;

		std::vector<Terrain*> *GetTerrains() const { return m_terrains; }
	};
}
