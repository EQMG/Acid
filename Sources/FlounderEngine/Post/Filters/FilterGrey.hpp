#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class F_EXPORT FilterGrey :
		public IPostFilter
	{
	private:
	public:
		FilterGrey(const int &subpass);

		~FilterGrey();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
