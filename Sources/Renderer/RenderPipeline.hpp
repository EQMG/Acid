#pragma once

#include "Commands/CommandBuffer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
	/// <summary>
	/// Represents a render pipeline that is used to render a type of pipeline.
	/// </summary>
	class ACID_EXPORT RenderPipeline
	{
	public:
		/// <summary>
		/// Creates a new render pipeline.
		/// </summary>
		/// <param name="graphicsStage"> The graphics stage this renderer will be used in. </param>
		explicit RenderPipeline(const GraphicsStage &graphicsStage) :
			m_graphicsStage(graphicsStage),
			m_enabled(true)
		{
		}

		~RenderPipeline() = default;

		/// <summary>
		/// Runs the render pipeline in the current renderpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to record render command into. </param>
		virtual void Render(const CommandBuffer &commandBuffer) = 0;

		const GraphicsStage &GetGraphicsStage() const { return m_graphicsStage; }

		const bool &IsEnabled() const { return m_enabled; };

		void SetEnabled(const bool &enable) { m_enabled = enable; }
	private:
		GraphicsStage m_graphicsStage;
		bool m_enabled;
	};
}
