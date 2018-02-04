#pragma once

#include <Entities/Entity.hpp>
#include <Models/Model.hpp>
#include <Textures/Texture.hpp>

using namespace Flounder;

class EntityRock1 :
	public Entity
{
public:
	EntityRock1(ISpatialStructure<Entity *> *structure, const Transform &transform);
};
