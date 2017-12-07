#pragma once

#include "../Filters/FilterBlurHorizontal.hpp"
#include "../Filters/FilterBlurVertical.hpp"
#include "../IPostPipeline.hpp"

namespace Flounder
{
	class PipelineGaussian :
		public IPostPipeline
	{
	private:
		FilterBlurHorizontal *m_filterBlurHorizontal;
		FilterBlurVertical *m_filterBlurVertical;
	public:
		PipelineGaussian(const int &subpass, const int &width, const int &height);

		PipelineGaussian(const int &subpass, const float &sizeScalar);

		~PipelineGaussian();

		void RenderPipeline(const VkCommandBuffer *commandBuffer) override;

		void SetScaleValue(const float &scaleValue) const;
	};
}
