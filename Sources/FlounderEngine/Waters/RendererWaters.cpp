#include "RendererWaters.hpp"

#include "../Scenes/Scenes.hpp"
#include "../Models/Model.hpp"
#include "../Textures/Texture.hpp"
#include "UbosWaters.hpp"

namespace Flounder
{
	RendererWaters::RendererWaters(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosWaters::UboScene))),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Waters/Water.vert", "Resources/Shaders/Waters/Water.frag"},
			VertexModel::GetBindingDescriptions(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE), { }))
	{
	}

	RendererWaters::~RendererWaters()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererWaters::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosWaters::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<WaterRender *> renderList = std::vector<WaterRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<WaterRender>(&renderList);

		for (auto waterRender : renderList)
		{
			waterRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
