#include "EntitySun.hpp"

#include <Entities/Components/ComponentModel.hpp>
#include <Entities/Components/ComponentMaterial.hpp>
#include <Entities/Components/ComponentGlow.hpp>
#include <Entities/Components/ComponentLight.hpp>
#include <Entities/Components/ComponentCelestial.hpp>
#include <Entities/Components/ComponentCollider.hpp>
#include <Models/Shapes/Sphere.hpp>

EntitySun::EntitySun(const Transform &transform, ISpatialStructure<Entity *> *structure) :
	Entity(transform, structure)
{
	AddComponent(new ComponentModel(
		Sphere::Resource(20, 20, 1.0f), // Model::Resource("Resources/Entities/Sun/Model.obj"),
		Texture::Resource("Resources/Entities/Sun/Diffuse.png")
	));
	AddComponent(new ComponentGlow(Texture::Resource("Resources/Entities/Sun/Glow.png")));
	AddComponent(new ComponentMaterial(0.5f, 0.5f, false, true, true));
	AddComponent(new ComponentLight(Light(Colour("#FFFFFF"), -1.0f), Vector3()));
	AddComponent(new ComponentCelestial(CelestialSun));
//	AddComponent(new ComponentCollider());
}
