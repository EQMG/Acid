#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterSepia :
		public IPostFilter
	{
	public:
		FilterSepia(const int &subpass);

		~FilterSepia();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
