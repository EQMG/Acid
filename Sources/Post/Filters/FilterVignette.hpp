#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterVignette :
		public IPostFilter
	{
	public:
		explicit FilterVignette(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
