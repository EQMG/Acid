#include <Entities/Components/ComponentModel.hpp>
#include <Entities/Components/ComponentMaterial.hpp>
#include <Entities/Components/ComponentCollider.hpp>

#include "EntityTreePine.hpp"

EntityTreePine::EntityTreePine(ISpatialStructure<Entity*> *structure, const Transform &transform) :
	Entity(structure, transform)
{
	AddComponent(new ComponentModel(
			Model::Resource("Resources/Entities/TreePine/Model.obj"),
			Texture::Resource("Resources/Entities/TreePine/Diffuse.png"))
	);
	AddComponent(new ComponentMaterial(0.0f, 0.0f));
	AddComponent(new ComponentCollider());
	// AddComponent(new ComponentSway(Resources::Create<Texture>("Resources/Entities/TreePine/Sway.png")));
}
