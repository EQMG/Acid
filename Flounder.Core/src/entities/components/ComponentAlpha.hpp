#pragma once

#include "../IComponent.hpp"

namespace Flounder
{
	class ComponentAlpha :
		public IComponent
	{
	private:
		float m_alpha;
	public:
		ComponentAlpha(const float &alpha);

		~ComponentAlpha();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		float GetAlpha() const { return m_alpha; }

		void SetAlpha(const float &alpha) { m_alpha = alpha; }
	};
}
