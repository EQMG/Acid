#pragma once

#include "../Entity.hpp"
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

		void Render(EntityRender *render) override;

		float GetAlpha() const { return m_alpha; }

		void SetAlpha(const float &alpha) { m_alpha = alpha; }
	};
}
