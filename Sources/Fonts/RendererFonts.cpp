#include "RendererFonts.hpp"

#include "Models/VertexModel.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	RendererFonts::RendererFonts(const GraphicsStage &graphicsStage) :
		RenderPipeline(graphicsStage),
		m_pipeline(PipelineGraphics(graphicsStage, {"Shaders/Fonts/Font.vert", "Shaders/Fonts/Font.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_READ_WRITE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, true, {})),
		m_uniformScene(UniformHandler())
	{
	}

	void RendererFonts::Render(const CommandBuffer &commandBuffer, const Camera &camera)
	{
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());

		m_pipeline.BindPipeline(commandBuffer);

		for (const auto &screenObject : Uis::Get()->GetObjects())
		{
			if (!screenObject->IsVisible())
			{
				continue;
			}

			Text *object = dynamic_cast<Text *>(screenObject);

			if (object != nullptr)
			{
				object->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
			}
		}
	}
}
