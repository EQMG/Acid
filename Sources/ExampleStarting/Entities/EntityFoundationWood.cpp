#include "EntityFoundationWood.hpp"

#include <Entities/Components/ComponentModel.hpp>
#include <Entities/Components/ComponentMaterial.hpp>
#include <Entities/Components/ComponentLight.hpp>
#include <Models/Shapes/Sphere.hpp>
#include <Entities/Components/ComponentCollider.hpp>

EntityFoundationWood::EntityFoundationWood(const Transform &transform, ISpatialStructure<Entity *> *structure) :
	Entity(transform, structure)
{
	AddComponent(new ComponentModel(
		Model::Resource("Resources/Entities/FoundationWood/Model.obj"),
		Texture::Resource("Resources/Entities/FoundationWood/Diffuse.png"))
	);
	AddComponent(new ComponentMaterial(0.0f, 0.0f));
//	AddComponent(new ComponentCollider());
}
