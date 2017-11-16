#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterGrey :
		public IPostFilter
	{
	public:
		FilterGrey(const int &subpass);

		~FilterGrey();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;
	};
}
