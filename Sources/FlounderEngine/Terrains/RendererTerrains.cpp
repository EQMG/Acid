#include "RendererTerrains.hpp"

#include "Scenes/Scenes.hpp"
#include "Models/Model.hpp"
#include "TerrainRender.hpp"
#include "UbosTerrains.hpp"

namespace Flounder
{
	RendererTerrains::RendererTerrains(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Terrains/Terrain.vert", "Resources/Shaders/Terrains/Terrain.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), {})),
		m_uniformScene(new UniformBuffer(sizeof(UbosTerrains::UboScene)))
	{
	}

	RendererTerrains::~RendererTerrains()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererTerrains::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosTerrains::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<TerrainRender *> renderList = std::vector<TerrainRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<TerrainRender>(&renderList);

		for (auto terrainRender : renderList)
		{
			terrainRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
