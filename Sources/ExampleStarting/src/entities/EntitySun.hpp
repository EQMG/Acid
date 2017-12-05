#pragma once

#include <Flounder.hpp>

using namespace Flounder;

class EntitySun :
	public Entity
{
private:
	static Model *g_model;
	static Texture *g_diffuse;
	static Texture *g_glowMap;
public:
	EntitySun(ISpatialStructure<Entity*> *structure, const Transform &transform);
};
