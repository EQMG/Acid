#pragma once

#include <Entities/Entity.hpp>
#include <Textures/Texture.hpp>

using namespace Flounder;

class EntitySun :
	public Entity
{
public:
	EntitySun(ISpatialStructure<Entity*> *structure, const Transform &transform);
};
