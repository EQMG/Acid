#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterBlurVertical :
		public IPostFilter
	{
	private:
		int m_heightValue;
		float m_scaleValue;
		bool m_fitToDisplay;
		float m_sizeScalar;
	public:
		FilterBlurVertical(const int &subpass, const float &sizeScalar);

		FilterBlurVertical(const int &subpass, const int &width, const int &height);

		~FilterBlurVertical();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;

		float GetScaleValue() const { return m_scaleValue; }

		void SetScaleValue(const float &scaleValue) { m_scaleValue = scaleValue; }
	};
}
