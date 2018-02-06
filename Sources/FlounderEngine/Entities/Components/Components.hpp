#pragma once

#include <map>
#include <string>
#include <functional>
#include "../IComponent.hpp"
#include "ComponentCelestial.hpp"
#include "ComponentCollider.hpp"
#include "ComponentCollision.hpp"
#include "ComponentGlow.hpp"
#include "ComponentLight.hpp"
#include "ComponentMaterial.hpp"
#include "ComponentModel.hpp"
#include "ComponentSway.hpp"

namespace Flounder
{
	class Components
	{
	public:
		static IComponent *CreateComponent(const std::string &name, ComponentPrefab *prefab)
		{
			if (name == "ComponentCelestial")
			{
				return new ComponentCelestial(prefab);
			}
			if (name == "ComponentCollider")
			{
				return new ComponentCollider(prefab);
			}
			if (name == "ComponentCollision")
			{
				return new ComponentCollision(prefab);
			}
			if (name == "ComponentGlow")
			{
				return new ComponentGlow(prefab);
			}
			if (name == "ComponentLight")
			{
				return new ComponentLight(prefab);
			}
			if (name == "ComponentMaterial")
			{
				return new ComponentMaterial(prefab);
			}
			if (name == "ComponentModel")
			{
				return new ComponentModel(prefab);
			}
			if (name == "ComponentSway")
			{
				return new ComponentSway(prefab);
			}
			return nullptr;
		}
	};
}