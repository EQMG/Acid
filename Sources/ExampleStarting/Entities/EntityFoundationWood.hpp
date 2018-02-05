#pragma once

#include <Entities/Entity.hpp>
#include <Models/Model.hpp>
#include <Textures/Texture.hpp>

using namespace Flounder;

class EntityFoundationWood :
	public Entity
{
public:
	EntityFoundationWood(const Transform &transform, ISpatialStructure<Entity *> *structure = nullptr);
};
