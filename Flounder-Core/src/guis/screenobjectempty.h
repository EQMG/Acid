#pragma once

#include "screenobject.h"

namespace flounder
{
	/// <summary>
	/// A gui object that has a empty update and delete method.
	/// </summary>
	class screenobjectempty :
		public screenobject
	{
	public:
		screenobjectempty(screenobject *parent, const vector2 &position, const vector2 &dimensions, const bool &inScreenCoords);

		~screenobjectempty();

		void updateObject() override;
	};
}
