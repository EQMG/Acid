#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterBloom1 :
		public IPostFilter
	{
	public:
		FilterBloom1(const int &subpass);

		~FilterBloom1();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;
	};
}
