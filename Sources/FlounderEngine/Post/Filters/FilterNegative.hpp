#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class FL_EXPORT FilterNegative :
		public IPostFilter
	{
	public:
		FilterNegative(const GraphicsStage &graphicsStage);

		~FilterNegative();

		void Render(const VkCommandBuffer &commandBuffer) override;
	};
}
