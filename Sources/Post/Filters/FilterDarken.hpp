#pragma once

#include "Post/IPostFilter.hpp"

namespace fl
{
	class FL_EXPORT FilterDarken :
		public IPostFilter
	{
	private:
		UniformHandler *m_uniformScene;

		float m_factor;
	public:
		FilterDarken(const GraphicsStage &graphicsStage);

		~FilterDarken();

		void Render(const CommandBuffer &commandBuffer) override;

		float GetFactor() const { return m_factor; }

		void SetFactor(const float &factor) { m_factor = factor; }
	};
}
