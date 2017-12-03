#pragma once

#include "../IPostFilter.hpp"
#include "../../renderer/buffers/UniformBuffer.hpp"

namespace Flounder
{
	class FilterFxaa :
		public IPostFilter
	{
	private:
		struct UboScene
		{
			float spanMax;
		};

		static const DescriptorType typeUboScene;
		static const DescriptorType typeSamplerColour;

		UniformBuffer *m_uniformScene;

		float m_spanMax;
	public:
		FilterFxaa(const int &subpass);

		~FilterFxaa();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;

		float GetSpanMax() const { return m_spanMax; }

		void SetSpanMax(const float &spanMax) { m_spanMax = spanMax; }
	};
}
