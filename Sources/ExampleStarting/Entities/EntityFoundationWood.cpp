#include "EntityFoundationWood.hpp"

#include <Entities/Components/ComponentModel.hpp>
#include <Entities/Components/ComponentMaterial.hpp>
#include <Entities/Components/ComponentLight.hpp>
#include <Models/Shapes/Sphere.hpp>
#include <Entities/Components/ComponentCollider.hpp>

EntityFoundationWood::EntityFoundationWood(ISpatialStructure<Entity *> *structure, const Transform &transform) :
	Entity(structure, transform)
{
	AddComponent(new ComponentModel(
		Model::Resource("Resources/Entities/FoundationWood/Model.obj"),
		Texture::Resource("Resources/Entities/FoundationWood/Diffuse.png"))
	);
	AddComponent(new ComponentMaterial(0.0f, 0.0f));
//	AddComponent(new ComponentCollider());
}
