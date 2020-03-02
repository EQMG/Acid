#pragma once

#include "Post/PostFilter.hpp"
#include "Post/Pipelines/PipelineBlur.hpp"

namespace acid {
class ACID_EXPORT FilterDof : public PostFilter {
public:
	explicit FilterDof(const Pipeline::Stage &pipelineStage, PipelineBlur *pipelineBlur, float focusPoint = 8.24f, float nearField = 0.3f,
		float nearTransition = 0.125f, float farField = 8.0f, float farTransition = 3.0f);

	void Render(const CommandBuffer &commandBuffer) override;

	const PipelineBlur *GetPipelineBlur() const { return pipelineBlur; }
	void SetPipelineBlur(PipelineBlur *pipelineBlur) { this->pipelineBlur = pipelineBlur; }

	float GetFocusPoint() const { return focusPoint; }
	void SetFocusPoint(float focusPoint) { this->focusPoint = focusPoint; }

	float GetNearField() const { return nearField; }
	void SetNearField(float nearField) { this->nearField = nearField; }

	float GetNearTransition() const { return nearTransition; }
	void SetNearTransition(float nearTransition) { this->nearTransition = nearTransition; }

	float GetFarField() const { return farField; }
	void SetFarField(float farField) { this->farField = farField; }

	float GetFarTransition() const { return farTransition; }
	void SetFarTransition(float farTransition) { this->farTransition = farTransition; }

private:
	PushHandler pushScene;

	PipelineBlur *pipelineBlur;

	float focusPoint;
	float nearField;
	float nearTransition;
	float farField;
	float farTransition;
};
}
