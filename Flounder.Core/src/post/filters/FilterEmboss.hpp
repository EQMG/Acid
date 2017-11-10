#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterEmboss :
		public IPostFilter
	{
	public:
		FilterEmboss();

		~FilterEmboss();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;
	};
}
