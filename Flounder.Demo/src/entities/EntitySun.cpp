#include "EntitySun.hpp"

Model *EntitySun::g_model = nullptr;
Texture *EntitySun::g_diffuse = nullptr;
Texture *EntitySun::g_glowMap = nullptr;

EntitySun::EntitySun(ISpatialStructure<Entity*> *structure, const Transform &transform) :
	Entity(structure, transform)
{
	if (g_model == nullptr)
	{
		g_model = new Model("res/entities/sun/model.obj");
	}

	if (g_diffuse == nullptr)
	{
		g_diffuse = new Texture("res/entities/sun/diffuse.png");
	}

	if (g_glowMap == nullptr)
	{
		g_glowMap = new Texture("res/entities/sun/glow.png");
	}

	AddComponent(new ComponentModel(g_model, g_diffuse));
	AddComponent(new ComponentGlow(g_glowMap));
	AddComponent(new ComponentLight(Light(Colour("#FFFFFF"), -1.0f), Vector3()));
	AddComponent(new ComponentSun());
}
