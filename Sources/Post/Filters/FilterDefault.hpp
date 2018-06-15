#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class FL_EXPORT FilterDefault :
		public IPostFilter
	{
	public:
		FilterDefault(const GraphicsStage &graphicsStage);

		~FilterDefault();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
