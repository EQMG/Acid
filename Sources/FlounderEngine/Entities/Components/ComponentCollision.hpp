#pragma once

#include "../IComponent.hpp"

namespace Flounder
{
	class F_EXPORT ComponentCollision :
		public IComponent
	{
	private:
	public:
		ComponentCollision();

		ComponentCollision(ComponentPrefab *prefab);

		~ComponentCollision();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		void Save(ComponentPrefab *prefab) override;

		std::string GetName() const override { return "ComponentCollision"; };

		Vector3 ResolveCollisions(const Vector3 &amount);
	};
}
