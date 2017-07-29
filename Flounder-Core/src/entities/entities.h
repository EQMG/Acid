#pragma once

#include <vector>

#include "../framework/framework.h"

#include "entity.h"

namespace flounder
{
	class entities :
		public imodule
	{
	private:
		std::vector<entity*> *m_entities;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static entities *get()
		{
			return static_cast<entities*>(framework::get()->getInstance("entities"));
		}

		/// <summary>
		/// Creates a new entities module.
		/// </summary>
		entities();

		/// <summary>
		/// Deconstructor for the entities module.
		/// </summary>
		~entities();

		void update() override;

		void add(entity *object);

		void remove(entity *object);

		/// <summary>
		/// The rendering entities from the container. Updated each update.
		/// </summary>
		/// <returns> The entities. </returns>
		inline std::vector<entity*> *getEntities() { return m_entities; };
	};
}
