#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class F_EXPORT FilterTone :
		public IPostFilter
	{
	public:
		FilterTone(const int &subpass);

		~FilterTone();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
