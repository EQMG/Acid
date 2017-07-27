#pragma once

#include "../framework/framework.h"

#include "fonttype.h"

namespace flounder
{
	/// <summary>
	/// A module used for that manages gui textures in a container.
	/// </summary>
	class fonts : 
		public imodule
	{
	public:
		fonttype *arial;
		fonttype *berlinSans;
		fonttype *cafeFrancoise;
		fonttype *candara;
		fonttype *segoe;

		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static fonts *get()
		{
			return static_cast<fonts*>(framework::get()->getInstance("fonts"));
		}

		/// <summary>
		/// Creates a new fonts module.
		/// </summary>
		fonts();

		/// <summary>
		/// Deconstructor for the fonts module.
		/// </summary>
		~fonts();

		void init() override;

		void update() override;
	};
}
