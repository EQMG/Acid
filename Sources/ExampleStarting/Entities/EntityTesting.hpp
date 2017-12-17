#pragma once

#include <Entities/Entity.hpp>
#include <Models/Model.hpp>
#include <Textures/Texture.hpp>

using namespace Flounder;

class EntityTesting :
	public Entity
{
private:
	static Model *g_model;
	static Texture *g_diffuse;
public:
	EntityTesting(ISpatialStructure<Entity*> *structure, const Transform &transform);
};
