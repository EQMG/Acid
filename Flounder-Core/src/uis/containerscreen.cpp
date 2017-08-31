#include "containerscreen.hpp"

namespace Flounder
{
	containerscreen::containerscreen(uiobject *parent, const vector2 &position, const vector2 &dimensions, const bool &inScreenCoords) :
		uiobject(parent, position, dimensions)
	{
		setInScreenCoords(inScreenCoords);
	}

	containerscreen::~containerscreen()
	{
	}

	void containerscreen::updateObject()
	{
	}
}
