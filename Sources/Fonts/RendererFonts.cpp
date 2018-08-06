#include "RendererFonts.hpp"

#include "Models/VertexModel.hpp"

namespace acid
{
	RendererFonts::RendererFonts(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Fonts/Font.vert", "Shaders/Fonts/Font.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {})))
	{
	}

	RendererFonts::~RendererFonts()
	{
	}

	void RendererFonts::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_pipeline.BindPipeline(commandBuffer);

		for (auto &screenObject : Uis::Get()->GetObjects())
		{
			if (!screenObject->IsVisible())
			{
				continue;
			}

			Text *object = dynamic_cast<Text *>(screenObject);

			if (object != nullptr)
			{
				object->CmdRender(commandBuffer, m_pipeline);
			}
		}
	}
}
