#pragma once

#include <Entities/Entity.hpp>
#include <Textures/Texture.hpp>

using namespace Flounder;

class EntityMoon :
	public Entity
{
public:
	EntityMoon(ISpatialStructure<Entity *> *structure, const Transform &transform);
};
