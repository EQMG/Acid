#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterGrey :
		public IPostFilter
	{
	private:
	public:
		FilterGrey(const int &subpass);

		~FilterGrey();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
