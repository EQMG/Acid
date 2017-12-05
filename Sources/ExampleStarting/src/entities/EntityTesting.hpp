#pragma once

#include <Flounder.hpp>

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
