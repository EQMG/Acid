#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class F_EXPORT FilterGrey :
		public IPostFilter
	{
	private:
	public:
		FilterGrey(const GraphicsStage &graphicsStage);

		~FilterGrey();

		void Render(const VkCommandBuffer &commandBuffer) override;
	};
}
