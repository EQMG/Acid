#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterGrey :
		public IPostFilter
	{
	public:
		FilterGrey();

		~FilterGrey();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;
	};
}
