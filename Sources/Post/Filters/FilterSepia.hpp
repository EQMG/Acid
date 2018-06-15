#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class FL_EXPORT FilterSepia :
		public IPostFilter
	{
	public:
		FilterSepia(const GraphicsStage &graphicsStage);

		~FilterSepia();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
