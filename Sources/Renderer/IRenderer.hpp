#pragma once

#include "Maths/Vector4.hpp"
#include "Scenes/ICamera.hpp"
#include "Commands/CommandBuffer.hpp"
#include "Pipelines/PipelineCreate.hpp"

namespace acid
{
	/// <summary>
	/// Represents a sub-renderer in the engine.
	/// </summary>
	class ACID_EXPORT IRenderer
	{
	private:
		GraphicsStage m_graphicsStage;
		bool m_enabled;
	public:
		/// <summary>
		/// Creates a new renderer.
		/// </summary>
		/// <param name="graphicsStage"> The graphics stage this renderer will be used in. </param>
		explicit IRenderer(const GraphicsStage &graphicsStage) :
			m_graphicsStage(graphicsStage),
			m_enabled(true)
		{
		}

		/// <summary>
		/// Called when the renderer is needed to be rendered.
		/// </summary>
		/// <param name="clipPlane"> The current clip plane. </param>
		/// <param name="camera"> The camera to be used when rendering. </param>
		virtual void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) = 0;

		GraphicsStage GetGraphicsStage() const { return m_graphicsStage; }

		bool IsEnabled() const { return m_enabled; };

		void SetEnabled(const bool &enable) { m_enabled = enable; }
	};
}
