#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterDefault :
		public IPostFilter
	{
	public:
		FilterDefault(const int &subpass);

		~FilterDefault();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
