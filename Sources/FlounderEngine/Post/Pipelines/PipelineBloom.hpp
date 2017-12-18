#pragma once

#include "../Filters/FilterBloom1.hpp"
#include "../Filters/FilterBloom2.hpp"
#include "../IPostPipeline.hpp"
#include "PipelineGaussian.hpp"

namespace Flounder
{
	class PipelineBloom :
		public IPostPipeline
	{
	private:
		FilterBloom1 *m_filterBloom1;
		PipelineGaussian *m_pipelineGaussian;
		FilterBloom2 *m_filterBloom2;
	public:
		PipelineBloom(const int &subpass);

		~PipelineBloom();

		void RenderPipeline(const VkCommandBuffer &commandBuffer) override;
	};
}
