#include "EntityTreePine.hpp"

#include <Entities/Components/ComponentModel.hpp>
#include <Entities/Components/ComponentMaterial.hpp>
#include <Entities/Components/ComponentCollider.hpp>

EntityTreePine::EntityTreePine(const Transform &transform, ISpatialStructure<Entity *> *structure) :
	Entity(transform, structure)
{
	AddComponent(new ComponentModel(
		Model::Resource("Resources/Entities/TreePine/Model.obj"),
		Texture::Resource("Resources/Entities/TreePine/Diffuse.png"))
	);
	// AddComponent(new ComponentSway(Texture::Resource("Resources/Entities/TreePine/Sway.png")));
	AddComponent(new ComponentMaterial(0.0f, 0.0f));
//	AddComponent(new ComponentCollider());
}
