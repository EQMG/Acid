#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterNegative :
		public IPostFilter
	{
	public:
		FilterNegative(const int &subpass);

		~FilterNegative();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
