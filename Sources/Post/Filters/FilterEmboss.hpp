#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterEmboss :
		public IPostFilter
	{
	public:
		FilterEmboss(const GraphicsStage &graphicsStage);

		~FilterEmboss();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
