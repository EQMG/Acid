#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class F_EXPORT FilterSepia :
		public IPostFilter
	{
	public:
		FilterSepia(const GraphicsStage &graphicsStage);

		~FilterSepia();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
