#pragma once

#include <Entities/Entity.hpp>
#include <Textures/Texture.hpp>

using namespace Flounder;

class EntityMoon :
	public Entity
{
public:
	EntityMoon(const Transform &transform, ISpatialStructure<Entity *> *structure = nullptr);
};
