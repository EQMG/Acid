#pragma once

#include "Post/Pipelines/PipelineBlur.hpp"
#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterDof : public PostFilter
{
  public:
	explicit FilterDof(const Pipeline::Stage& pipelineStage, PipelineBlur* pipelineBlur, const float& focusPoint = 8.24f, const float& nearField = 0.3f, const float& nearTransition = 0.125f, const float& farField = 8.0f, const float& farTransition = 3.0f);

	void Render(const CommandBuffer& commandBuffer) override;

	const PipelineBlur* GetPipelineBlur() const
	{
		return m_pipelineBlur;
	}

	void SetPipelineBlur(PipelineBlur* pipelineBlur)
	{
		m_pipelineBlur = pipelineBlur;
	}

	const float& GetFocusPoint() const
	{
		return m_focusPoint;
	}

	void SetFocusPoint(const float& focusPoint)
	{
		m_focusPoint = focusPoint;
	}

	const float& GetNearField() const
	{
		return m_nearField;
	}

	void SetNearField(const float& nearField)
	{
		m_nearField = nearField;
	}

	const float& GetNearTransition() const
	{
		return m_nearTransition;
	}

	void SetNearTransition(const float& nearTransition)
	{
		m_nearTransition = nearTransition;
	}

	const float& GetFarField() const
	{
		return m_farField;
	}

	void SetFarField(const float& farField)
	{
		m_farField = farField;
	}

	const float& GetFarTransition() const
	{
		return m_farTransition;
	}

	void SetFarTransition(const float& farTransition)
	{
		m_farTransition = farTransition;
	}

  private:
	PushHandler m_pushScene;

	PipelineBlur* m_pipelineBlur;

	float m_focusPoint;
	float m_nearField;
	float m_nearTransition;
	float m_farField;
	float m_farTransition;
};
}
