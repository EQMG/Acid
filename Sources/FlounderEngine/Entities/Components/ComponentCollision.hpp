#pragma once

#include "../../Objects/Component.hpp"
#include "../EntityRender.hpp"

namespace Flounder
{
	class F_EXPORT ComponentCollision :
		public Component
	{
	private:
	public:
		ComponentCollision();

		ComponentCollision(ComponentPrefab *prefab);

		~ComponentCollision();

		void Update() override;

		void CmdRender(EntityRender *entityRender);

		std::string GetName() const override { return "ComponentCollision"; };

		Vector3 ResolveCollisions(const Vector3 &amount);
	};
}
