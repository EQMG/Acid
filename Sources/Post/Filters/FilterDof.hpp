#pragma once

#include "Post/PostFilter.hpp"
#include "Post/Pipelines/PipelineBlur.hpp"

namespace acid {
class ACID_EXPORT FilterDof : public PostFilter {
public:
	explicit FilterDof(const Pipeline::Stage &pipelineStage, PipelineBlur *pipelineBlur, float focusPoint = 8.24f, float nearField = 0.3f,
		float nearTransition = 0.125f, float farField = 8.0f, float farTransition = 3.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	const PipelineBlur *GetPipelineBlur() const { return m_pipelineBlur; }
	void SetPipelineBlur(PipelineBlur *pipelineBlur) { m_pipelineBlur = pipelineBlur; }

	float GetFocusPoint() const { return m_focusPoint; }
	void SetFocusPoint(float focusPoint) { m_focusPoint = focusPoint; }

	float GetNearField() const { return m_nearField; }
	void SetNearField(float nearField) { m_nearField = nearField; }

	float GetNearTransition() const { return m_nearTransition; }
	void SetNearTransition(float nearTransition) { m_nearTransition = nearTransition; }

	float GetFarField() const { return m_farField; }
	void SetFarField(float farField) { m_farField = farField; }

	float GetFarTransition() const { return m_farTransition; }
	void SetFarTransition(float farTransition) { m_farTransition = farTransition; }

private:
	PushHandler m_pushScene;

	PipelineBlur *m_pipelineBlur;

	float m_focusPoint;
	float m_nearField;
	float m_nearTransition;
	float m_farField;
	float m_farTransition;
};
}
