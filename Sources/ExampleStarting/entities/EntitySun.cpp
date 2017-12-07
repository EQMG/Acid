#include "EntitySun.hpp"

Model *EntitySun::g_model = nullptr;
Texture *EntitySun::g_diffuse = nullptr;
Texture *EntitySun::g_glowMap = nullptr;

EntitySun::EntitySun(ISpatialStructure<Entity*> *structure, const Transform &transform) :
	Entity(structure, transform)
{
	if (g_model == nullptr)
	{
		g_model = new Model("Resources/Entities/Sun/Model.obj");
	}

	if (g_diffuse == nullptr)
	{
		g_diffuse = new Texture("Resources/Entities/Sun/Diffuse.png");
	}

	if (g_glowMap == nullptr)
	{
		g_glowMap = new Texture("Resources/Entities/Sun/Glow.png");
	}

	AddComponent(new ComponentModel(g_model, g_diffuse));
	AddComponent(new ComponentMaterial(0.5f, 0.5f));
	AddComponent(new ComponentGlow(g_glowMap));
	AddComponent(new ComponentLight(Light(Colour("#FFFFFF"), -1.0f), Vector3()));
	AddComponent(new ComponentSun());
}
