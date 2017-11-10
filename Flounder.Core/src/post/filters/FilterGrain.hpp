#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterGrain :
		public IPostFilter
	{
	private:
		float m_strength;
	public:
		FilterGrain(const float &strength = 2.3f);

		~FilterGrain();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;

		float GetStrength() const { return m_strength; }

		void SetStrength(const float &strength) { m_strength = strength; }
	};
}
