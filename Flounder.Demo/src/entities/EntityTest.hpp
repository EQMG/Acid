#pragma once

#include <Flounder.hpp>

using namespace Flounder;

class EntityTest :
	public Entity
{
public:
	EntityTest(ISpatialStructure<Entity*> *structure, const Vector3 &position, const Vector3 &rotation) :
		Entity(structure, position, rotation)
	{
		AddComponent(new ComponentAlpha(1.0f));
	}
};
