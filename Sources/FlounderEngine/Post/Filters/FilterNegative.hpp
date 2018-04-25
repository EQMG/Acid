#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class F_EXPORT FilterNegative :
		public IPostFilter
	{
	public:
		FilterNegative(const GraphicsStage &graphicsStage);

		~FilterNegative();

		void Render(const VkCommandBuffer &commandBuffer) override;
	};
}
