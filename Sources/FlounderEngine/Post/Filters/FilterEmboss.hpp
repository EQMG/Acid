#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class F_EXPORT FilterEmboss :
		public IPostFilter
	{
	public:
		FilterEmboss(const int &subpass);

		~FilterEmboss();

		void RenderFilter(const VkCommandBuffer &commandBuffer) override;
	};
}
