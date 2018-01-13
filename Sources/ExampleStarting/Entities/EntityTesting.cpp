#include <Entities/Components/ComponentModel.hpp>
#include <Entities/Components/ComponentMaterial.hpp>
#include <Entities/Components/ComponentLight.hpp>
#include <Models/Shapes/Cube.hpp>
#include <Models/Shapes/Sphere.hpp>

#include "EntityTesting.hpp"

Model *EntityTesting::g_model = nullptr;
Texture *EntityTesting::g_diffuse = nullptr;

EntityTesting::EntityTesting(ISpatialStructure<Entity*> *structure, const Transform &transform) :
	Entity(structure, transform)
{
	if (g_model == nullptr)
	{
		g_model = new Cube(1.0f, 1.0f, 1.0f); // Sphere(18, 18, 1.0f) // CreateCube::Create(1.0f, 0.5f, 2.0f); // new Model("Resources/Models/ModelSphere.obj");
	}

	if (g_diffuse == nullptr)
	{
		g_diffuse = new Texture("Resources/Guis/White.png");
	}

	AddComponent(new ComponentModel(g_model, g_diffuse));
	AddComponent(new ComponentMaterial(0.0f, 0.0f));
	AddComponent(new ComponentLight(Light(Colour(0.0f, 0.3f, 1.0f), 10.0f), Vector3(0.0f, 3.0f, 0.0f)));
}
