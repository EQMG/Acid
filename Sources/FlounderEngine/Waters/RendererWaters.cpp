#include "RendererWaters.hpp"

#include "Scenes/Scenes.hpp"
#include "Models/Model.hpp"
#include "WaterRender.hpp"

namespace Flounder
{
	RendererWaters::RendererWaters(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Waters/Water.vert", "Resources/Shaders/Waters/Water.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE), {})),
		m_uniformScene(new UniformHandler())
	{
	}

	RendererWaters::~RendererWaters()
	{
		delete m_pipeline;
		delete m_uniformScene;
	}

	void RendererWaters::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene->Push("projection", *camera.GetProjectionMatrix());
		m_uniformScene->Push("view", *camera.GetViewMatrix());

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<WaterRender *> renderList = std::vector<WaterRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<WaterRender>(&renderList);

		for (auto waterRender : renderList)
		{
			waterRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
