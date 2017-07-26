#pragma once

#include "../framework/framework.h"

#include "fonttype.h"

namespace flounder
{
	/// <summary>
	/// A module used for that manages gui textures in a container.
	/// </summary>
	class fonts : public imodule
	{
	private:
		static fonttype *const ARIAL;
		static fonttype *const BERLIN_SANS;
		static fonttype *const CAFE_FRANCOISE;
		static fonttype *const CANDARA;
		static fonttype *const SEGOE;
	public:
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
