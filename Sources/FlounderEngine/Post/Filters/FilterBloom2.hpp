#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterBloom2 :
		public IPostFilter
	{
	public:
		FilterBloom2(const int &subpass);

		~FilterBloom2();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
