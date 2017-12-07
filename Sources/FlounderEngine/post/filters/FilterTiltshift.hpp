#pragma once

#include "../IPostFilter.hpp"
#include "../../Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	class FilterTiltshift :
		public IPostFilter
	{
	private:
		struct UboScene
		{
			float blurAmount;
			float centre;
			float stepSize;
			float steps;
		};

		static const DescriptorType typeUboScene;
		static const DescriptorType typeSamplerColour;

		UniformBuffer *m_uniformScene;

		float m_blurAmount;
		float m_centre;
		float m_stepSize;
		float m_steps;
	public:
		FilterTiltshift(const int &subpass);

		~FilterTiltshift();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;

		float GetBlurAmount() const { return m_blurAmount; }

		void SetBlurAmount(const float &blurAmount) { m_blurAmount = blurAmount; }

		float GetCentre() const { return m_centre; }

		void SetCentre(const float &centre) { m_centre = centre; }

		float GetStepSize() const { return m_stepSize; }

		void SetStepSize(const float &stepSize) { m_stepSize = stepSize; }

		float GetSteps() const { return m_steps; }

		void SetSteps(const float &steps) { m_steps = steps; }
	};
}
