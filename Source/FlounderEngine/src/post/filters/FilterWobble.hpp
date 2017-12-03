#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterWobble :
		public IPostFilter
	{
	private:
		float m_wobbleSpeed;
		float m_wobbleAmount;
	public:
		FilterWobble(const int &subpass);

		~FilterWobble();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;

		float GetWobbleSpeed() const { return m_wobbleSpeed; }

		void SetWobbleSpeed(const float &wobbleSpeed) { m_wobbleSpeed = wobbleSpeed; }
	};
}
