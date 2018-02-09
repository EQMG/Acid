#pragma once

#include <map>
#include <string>
#include <functional>
#include "../IComponent.hpp"
#include "../Components/ComponentCelestial.hpp"
#include "../Components/ComponentCollider.hpp"
#include "../Components/ComponentCollision.hpp"
#include "../Components/ComponentDiffuse.hpp"
#include "../Components/ComponentLight.hpp"
#include "../Components/ComponentMaterial.hpp"
#include "../Components/ComponentModel.hpp"
#include "../Components/ComponentNormal.hpp"
#include "../Components/ComponentSway.hpp"

namespace Flounder
{
	class Components
	{
	private:
	public:
		static IComponent *CreateComponent(const std::string &name, ComponentPrefab *prefab)
		{
			// TODO: Recreate function to be more modular.
			if (name == "ComponentCelestial") { return new ComponentCelestial(prefab); }
			if (name == "ComponentCollider") { return new ComponentCollider(prefab); }
			if (name == "ComponentCollision") { return new ComponentCollision(prefab); }
			if (name == "ComponentDiffuse") { return new ComponentDiffuse(prefab); }
			if (name == "ComponentLight") { return new ComponentLight(prefab); }
			if (name == "ComponentMaterial") { return new ComponentMaterial(prefab); }
			if (name == "ComponentModel") { return new ComponentModel(prefab); }
			if (name == "ComponentNormal") { return new ComponentNormal(prefab); }
			if (name == "ComponentSway") { return new ComponentSway(prefab); }
			return nullptr;
		}

		static void AttachAllTo(EntityPrefab *entityPrefab, Entity *entity)
		{
			for (const auto cName : entityPrefab->GetComponents())
			{
				if (FormatString::StartsWith(cName, "#"))
				{
					continue;
				}

				ComponentPrefab *cPrefab = entityPrefab->GetComponentData(cName);

				if (cPrefab == nullptr)
				{
					continue;
				}

				IComponent *component = Components::CreateComponent(cName, cPrefab);

				if (component == nullptr)
				{
					continue;
				}

				if (entity != nullptr)
				{
					entity->AddComponent(component);
				}
			}
		}
	};
}