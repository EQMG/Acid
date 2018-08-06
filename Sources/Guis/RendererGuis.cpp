#include "RendererGuis.hpp"

#include "Models/VertexModel.hpp"
#include "Uis/Uis.hpp"
#include "Gui.hpp"

namespace acid
{
	RendererGuis::RendererGuis(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Guis/Gui.vert", "Shaders/Guis/Gui.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {})))
	{
	}

	RendererGuis::~RendererGuis()
	{
	}

	void RendererGuis::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_pipeline.BindPipeline(commandBuffer);

		for (auto &screenObject : Uis::Get()->GetObjects())
		{
			if (!screenObject->IsVisible())
			{
				continue;
			}

			Gui *object = dynamic_cast<Gui *>(screenObject);

			if (object != nullptr)
			{
				object->CmdRender(commandBuffer, m_pipeline);
			}
		}
	}
}
