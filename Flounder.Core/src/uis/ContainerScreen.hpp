#pragma once

#include "uiobject.hpp"

namespace Flounder
{
	/// <summary>
	/// A gui object that has a empty update and delete method.
	/// </summary>
	class containerscreen :
		public UiObject
	{
	public:
		containerscreen(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, const bool &inScreenCoords);

		~containerscreen();

		void UpdateObject() override;
	};
}
