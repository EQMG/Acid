#pragma once

#include <flounder.hpp>

using namespace Flounder;

class entitytest :
	public entity
{
public:
	entitytest(ispatialstructure<entity*> *structure, const vector3 &position, const vector3 &rotation) :
		entity(structure, position, rotation)
	{
		addComponent(new componentalpha(1.0f));
	}
};
