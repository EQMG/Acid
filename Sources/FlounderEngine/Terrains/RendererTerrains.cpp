#include "RendererTerrains.hpp"

#include "Scenes/Scenes.hpp"
#include "Models/Model.hpp"
#include "TerrainRender.hpp"

namespace Flounder
{
	RendererTerrains::RendererTerrains(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Terrains/Terrain.vert", "Resources/Shaders/Terrains/Terrain.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), {})),
		m_uniformScene(new UniformHandler())
	{
	}

	RendererTerrains::~RendererTerrains()
	{
		delete m_pipeline;
		delete m_uniformScene;
	}

	void RendererTerrains::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene->Push("projection", *camera.GetProjectionMatrix());
		m_uniformScene->Push("view", *camera.GetViewMatrix());

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<TerrainRender *> renderList = std::vector<TerrainRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<TerrainRender>(&renderList);

		for (auto terrainRender : renderList)
		{
			terrainRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
