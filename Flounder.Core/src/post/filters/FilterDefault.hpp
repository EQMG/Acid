#pragma once

#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterDefault :
		public IPostFilter
	{
	public:
		FilterDefault();

		~FilterDefault();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;
	};
}
