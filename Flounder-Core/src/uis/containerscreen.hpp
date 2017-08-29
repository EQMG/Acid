#pragma once

#include "uiobject.hpp"

namespace flounder
{
	/// <summary>
	/// A gui object that has a empty update and delete method.
	/// </summary>
	class containerscreen :
		public uiobject
	{
	public:
		containerscreen(uiobject *parent, const vector2 &position, const vector2 &dimensions, const bool &inScreenCoords);

		~containerscreen();

		void updateObject() override;
	};
}
