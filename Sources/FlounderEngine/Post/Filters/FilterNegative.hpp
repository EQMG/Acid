#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class F_EXPORT FilterNegative :
		public IPostFilter
	{
	public:
		FilterNegative(const int &subpass);

		~FilterNegative();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
