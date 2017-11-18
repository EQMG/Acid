#pragma once

#include "../IComponent.hpp"

namespace Flounder
{
	class ComponentCollider :
		public IComponent
	{
	private:
		ICollider *m_collider;
	public:
		ComponentCollider();

		~ComponentCollider();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		ICollider *GetCollider() const { return m_collider; }
	};
}
