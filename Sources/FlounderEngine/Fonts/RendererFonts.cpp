#include "RendererFonts.hpp"

namespace Flounder
{
	RendererFonts::RendererFonts(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Fonts/Font.vert", "Resources/Shaders/Fonts/Font.frag"},
			VertexModel::GetVertexInput(), PIPELINE_POLYGON_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), { }))
	{
	}

	RendererFonts::~RendererFonts()
	{
		delete m_pipeline;
	}

	void RendererFonts::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_pipeline->BindPipeline(commandBuffer);

		for (auto screenObject : *Uis::Get()->GetObjects())
		{
			if (!screenObject->IsVisible())
			{
				continue;
			}

			Text *object = dynamic_cast<Text *>(screenObject);

			if (object != nullptr)
			{
				object->CmdRender(commandBuffer, *m_pipeline);
			}
		}
	}
}
