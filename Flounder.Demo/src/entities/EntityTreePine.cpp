#include "EntityTreePine.hpp"

Model *EntityTreePine::g_model = nullptr;
Texture *EntityTreePine::g_diffuse = nullptr;
Texture *EntityTreePine::g_swayMap = nullptr;

EntityTreePine::EntityTreePine(ISpatialStructure<Entity *> *structure, const Transform &transform) :
	Entity(structure, transform)
{
	if (g_model == nullptr)
	{
		g_model = new Model("res/entities/treePine/model.obj");
	}

	if (g_diffuse == nullptr)
	{
		g_diffuse = new Texture("res/entities/treePine/diffuse.png");
	}

	if (g_swayMap == nullptr)
	{
		g_swayMap = new Texture("res/entities/treePine/sway.png");
	}

	AddComponent(new ComponentAlpha(1.0f));
	AddComponent(new ComponentModel(g_model, g_diffuse));
	AddComponent(new ComponentSway(g_swayMap));
}
