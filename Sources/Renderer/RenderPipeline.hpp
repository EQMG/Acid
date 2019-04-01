#pragma once

#include "Commands/CommandBuffer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
/**
 * Represents a render pipeline that is used to render a type of pipeline.
 */
class ACID_EXPORT RenderPipeline
{
public:
	/**
	 * Creates a new render pipeline.
	 * @param stage The stage this renderer will be used in.
	 */
	explicit RenderPipeline(Pipeline::Stage stage) :
		m_stage(std::move(stage)),
		m_enabled(true)
	{
	}

	virtual ~RenderPipeline() = default;

	/**
	 * Runs the render pipeline in the current renderpass.
	 * @param commandBuffer The command buffer to record render command into.
	 */
	virtual void Render(const CommandBuffer &commandBuffer) = 0;

	const Pipeline::Stage &GetStage() const { return m_stage; }

	const bool &IsEnabled() const { return m_enabled; };

	void SetEnabled(const bool &enable) { m_enabled = enable; }

private:
	Pipeline::Stage m_stage;
	bool m_enabled;
};
}
