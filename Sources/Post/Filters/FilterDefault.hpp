#pragma once

#include "Post/PostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterDefault :
		public PostFilter
	{
	private:
		bool m_lastFilter;
	public:
		explicit FilterDefault(const GraphicsStage &graphicsStage, const bool &lastFilter = false);

		void Render(const CommandBuffer &commandBuffer, const Camera &camera) override;
	};
}
