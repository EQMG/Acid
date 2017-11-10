#pragma once

#include "../filters/FilterBloom1.hpp"
#include "../filters/FilterBloom2.hpp"
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
		PipelineBloom();

		~PipelineBloom();

		void RenderPipeline(const VkCommandBuffer *commandBuffer) override;
	};
}
