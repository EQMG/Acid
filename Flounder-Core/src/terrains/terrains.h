#pragma once

#include <vector>

#include "../framework/framework.h"

#include "terrain.h"

namespace flounder
{
	/// <summary>
	/// A module used for managing terrains in 3D worlds.
	/// </summary>
	class terrains :
		public imodule
	{
	private:
		std::vector<terrain*> *m_terrains;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static terrains *get()
		{
			return static_cast<terrains*>(framework::get()->getInstance("terrains"));
		}

		/// <summary>
		/// Creates a new terrains module.
		/// </summary>
		terrains();

		/// <summary>
		/// Deconstructor for the terrains module.
		/// </summary>
		~terrains();

		void update() override;

		std::vector<terrain*> *getTerrains() const { return m_terrains; }
	};
}
