#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterDefault :
		public IPostFilter
	{
	private:
		bool m_lastFilter;
	public:
		explicit FilterDefault(const GraphicsStage &graphicsStage, const bool &lastFilter = false);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
