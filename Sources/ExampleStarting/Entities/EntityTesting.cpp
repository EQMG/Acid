#include "EntityTesting.hpp"

#include <Entities/Components/ComponentModel.hpp>
#include <Entities/Components/ComponentMaterial.hpp>
#include <Entities/Components/ComponentLight.hpp>
#include <Models/Shapes/Sphere.hpp>
#include <Entities/Components/ComponentCollider.hpp>

EntityTesting::EntityTesting(ISpatialStructure<Entity *> *structure, const Transform &transform) :
	Entity(structure, transform)
{
	AddComponent(new ComponentModel(
		Sphere::Resource(20, 20, 1.0f), // Cube::Resource("1.0, 1.0, 1.0")
		Texture::Resource("Resources/Guis/Undefined.png")
	));
	AddComponent(new ComponentMaterial(0.002f, 0.3f));
	AddComponent(new ComponentLight(Light(Colour(0.2f, 0.1f, 0.8f), 1.333f), Vector3(0.0f, 3.0f, 0.0f)));
	AddComponent(new ComponentCollider());
}
