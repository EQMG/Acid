#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterEmboss :
		public PostFilter
	{
	public:
		explicit FilterEmboss(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Camera &camera) override;
	};
}
