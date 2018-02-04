#include "EntityMoon.hpp"

#include <Entities/Components/ComponentModel.hpp>
#include <Entities/Components/ComponentMaterial.hpp>
#include <Entities/Components/ComponentGlow.hpp>
#include <Entities/Components/ComponentLight.hpp>
#include <Entities/Components/ComponentCelestial.hpp>
#include <Entities/Components/ComponentCollider.hpp>
#include <Models/Shapes/Sphere.hpp>

EntityMoon::EntityMoon(ISpatialStructure<Entity *> *structure, const Transform &transform) :
	Entity(structure, transform)
{
	AddComponent(new ComponentModel(
		Sphere::Resource(16, 16, 1.0f),
		Texture::Resource("Resources/Entities/Moon/Diffuse.png")
	));
	AddComponent(new ComponentMaterial(0.5f, 0.5f, false, true, true));
	AddComponent(new ComponentGlow(Texture::Resource("Resources/Entities/Sun/Glow.png")));
	AddComponent(new ComponentLight(Light(Colour("#1E1E1E"), -1.0f), Vector3()));
	AddComponent(new ComponentCelestial(CelestialMoon));
//	AddComponent(new ComponentCollider());
}
