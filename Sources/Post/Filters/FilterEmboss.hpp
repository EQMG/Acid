#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class FL_EXPORT FilterEmboss :
		public IPostFilter
	{
	public:
		FilterEmboss(const GraphicsStage &graphicsStage);

		~FilterEmboss();

		void Render(const CommandBuffer &commandBuffer) override;
	};
}
