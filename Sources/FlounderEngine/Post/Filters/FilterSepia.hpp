#pragma once

#include "Post/IPostFilter.hpp"

namespace Flounder
{
	class F_EXPORT FilterSepia :
		public IPostFilter
	{
	public:
		FilterSepia(const GraphicsStage &graphicsStage);

		~FilterSepia();

		void Render(const VkCommandBuffer &commandBuffer) override;
	};
}
