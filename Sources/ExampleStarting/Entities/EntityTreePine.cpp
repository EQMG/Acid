#include <Entities/Components/ComponentModel.hpp>
#include <Entities/Components/ComponentMaterial.hpp>
#include <Entities/Components/ComponentCollider.hpp>

#include "EntityTreePine.hpp"

Model *EntityTreePine::g_model = nullptr;
Texture *EntityTreePine::g_diffuse = nullptr;
Texture *EntityTreePine::g_swayMap = nullptr;

EntityTreePine::EntityTreePine(ISpatialStructure<Entity*> *structure, const Transform &transform) :
	Entity(structure, transform)
{
	if (g_model == nullptr)
	{
		g_model = new Model("Resources/Entities/TreePine/Model.obj");
	}

	if (g_diffuse == nullptr)
	{
		g_diffuse = new Texture("Resources/Entities/TreePine/Diffuse.png");
	}

	if (g_swayMap == nullptr)
	{
		g_swayMap = new Texture("Resources/Entities/TreePine/Sway.png");
	}

	AddComponent(new ComponentModel(g_model, g_diffuse));
	AddComponent(new ComponentMaterial(0.0f, 0.0f));
	AddComponent(new ComponentCollider());
	// AddComponent(new ComponentSway(g_swayMap));
}
