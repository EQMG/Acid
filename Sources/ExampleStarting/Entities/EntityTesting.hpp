#pragma once

#include <Entities/Entity.hpp>
#include <Models/Model.hpp>
#include <Textures/Texture.hpp>

using namespace Flounder;

class EntityTesting :
	public Entity
{
public:
	EntityTesting(ISpatialStructure<Entity *> *structure, const Transform &transform);
};
