#include "screenobjectempty.h"

namespace flounder
{
	screenobjectempty::screenobjectempty(screenobject *parent, const vector2 &position, const vector2 &dimensions, const bool &inScreenCoords) :
		screenobject(parent, position, dimensions)
	{
		setInScreenCoords(inScreenCoords);
	}

	screenobjectempty::~screenobjectempty()
	{
	}

	void screenobjectempty::updateObject()
	{
	}
}
