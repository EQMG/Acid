#include "RendererGuis.hpp"

#include "Models/VertexModel.hpp"
#include "Uis/Uis.hpp"
#include "Gui.hpp"

namespace fl
{
	RendererGuis::RendererGuis(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Guis/Gui.vert", "Resources/Shaders/Guis/Gui.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON_NO_DEPTH, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK), {}))
	{
	}

	RendererGuis::~RendererGuis()
	{
		delete m_pipeline;
	}

	void RendererGuis::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_pipeline->BindPipeline(commandBuffer);

		for (auto screenObject : Uis::Get()->GetObjects())
		{
			if (!screenObject->IsVisible())
			{
				continue;
			}

			Gui *object = dynamic_cast<Gui *>(screenObject);

			if (object != nullptr)
			{
				object->CmdRender(commandBuffer, *m_pipeline);
			}
		}
	}
}
