#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterNegative :
		public PostFilter
	{
	public:
		explicit FilterNegative(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer) override;
	};
}
