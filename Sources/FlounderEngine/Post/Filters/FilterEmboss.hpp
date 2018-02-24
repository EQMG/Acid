#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class F_EXPORT FilterEmboss :
		public IPostFilter
	{
	public:
		FilterEmboss(const GraphicsStage &graphicsStage);

		~FilterEmboss();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
