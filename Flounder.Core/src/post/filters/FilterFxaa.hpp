#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterFxaa :
		public IPostFilter
	{
	private:
		float m_spanMax;
	public:
		FilterFxaa(const float &spanMax = 8.0f);

		~FilterFxaa();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;

		float GetSpanMax() const { return m_spanMax; }

		void SetSpanMax(const float &spanMax) { m_spanMax = spanMax; }
	};
}
