#pragma once

#include "Maths/Colour.hpp"
#include "Post/IPostFilter.hpp"
#include "Post/Pipelines/PipelineGaussian.hpp"

namespace acid
{
	class ACID_EXPORT FilterDof :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		PipelineGaussian *m_pipelineGaussian;

		float m_focusPoint;
		float m_nearField;
		float m_nearTransition;
		float m_farField;
		float m_farTransition;
	public:
		explicit FilterDof(const GraphicsStage &graphicsStage, PipelineGaussian *pipelineGaussian, const float &focusPoint = 8.24f, const float &nearField = 0.3f, const float &nearTransition = 0.125f,
			const float &farField = 8.0f, const float &farTransition = 3.0f);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		float GetFocusPoint() const { return m_focusPoint; }

		void SetFocusPoint(const float &focusPoint) { m_focusPoint = focusPoint; }

		float GetNearField() const { return m_nearField; }

		void SetNearField(const float &nearField) { m_nearField = nearField; }

		float GetNearTransition() const { return m_nearTransition; }

		void SetNearTransition(const float &nearTransition) { m_nearTransition = nearTransition; }

		float GetFarField() const { return m_farField; }

		void SetFarField(const float &farField) { m_farField = farField; }

		float GetFarTransition() const { return m_farTransition; }

		void SetFarTransition(const float &farTransition) { m_farTransition = farTransition; }
	};
}
