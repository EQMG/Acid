#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterSepia :
		public PostFilter
	{
	public:
		explicit FilterSepia(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer) override;
	};
}
