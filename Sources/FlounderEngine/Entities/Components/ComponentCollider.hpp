#pragma once

#include "../../Objects/Component.hpp"
#include "../EntityRender.hpp"

namespace Flounder
{
	struct F_EXPORT Constraint3
	{
		bool x, y, z;
	};

	class F_EXPORT ComponentCollider :
		public Component
	{
	private:
		ICollider *m_collider;
		float m_mass;
		float m_drag;
		bool m_useGravity;
		Constraint3 m_freezePosition;
		Constraint3 m_freezeRotation;
	public:
		ComponentCollider();

		ComponentCollider(ComponentPrefab *prefab);

		~ComponentCollider();

		void Update() override;

		void CmdRender(EntityRender *entityRender);

		std::string GetName() const override { return "ComponentCollider"; };

		ICollider *GetCollider() const { return m_collider; }
	};
}
