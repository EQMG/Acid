#include "containerscreen.hpp"

namespace Flounder
{
	containerscreen::containerscreen(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, const bool &inScreenCoords) :
		UiObject(parent, position, dimensions)
	{
		SetInScreenCoords(inScreenCoords);
	}

	containerscreen::~containerscreen()
	{
	}

	void containerscreen::UpdateObject()
	{
	}
}
