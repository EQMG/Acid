#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterEmboss :
		public IPostFilter
	{
	public:
		FilterEmboss(const int &subpass);

		~FilterEmboss();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
