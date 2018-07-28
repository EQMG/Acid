#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterGrey :
		public IPostFilter
	{
	public:
		FilterGrey(const GraphicsStage &graphicsStage);

		~FilterGrey();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
