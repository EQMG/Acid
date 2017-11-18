#pragma once

#include <Flounder.hpp>

using namespace Flounder;

class EntityTreePine :
	public Entity
{
private:
	static Model *g_model;
	static Texture *g_diffuse;
	static Texture *g_swayMap;
public:
	EntityTreePine(ISpatialStructure<Entity*> *structure, const Transform &transform);
};
