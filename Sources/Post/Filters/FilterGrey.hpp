#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class FL_EXPORT FilterGrey :
		public IPostFilter
	{
	public:
		FilterGrey(const GraphicsStage &graphicsStage);

		~FilterGrey();

		void Render(const VkCommandBuffer &commandBuffer) override;
	};
}
