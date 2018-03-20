#include "RendererTerrains.hpp"

#include "../Scenes/Scenes.hpp"
#include "../Models/Model.hpp"
#include "Terrains.hpp"
#include "UbosTerrains.hpp"

namespace Flounder
{
	const PipelineCreate PIPELINE_CREATE =
		{
			PIPELINE_MRT, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_BACK_BIT, // cullModeFlags

			VertexModel::GetBindingDescriptions(), // vertexBindingDescriptions
			VertexModel::GetAttributeDescriptions(), // vertexAttributeDescriptions

			{
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT), // uboScene
				UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL) // uboObject
			}, // descriptors

			{
				"Resources/Shaders/Terrains/Terrain.vert.spv", "Resources/Shaders/Terrains/Terrain.frag.spv"
			} // shaderStages
		};

	RendererTerrains::RendererTerrains(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosTerrains::UboScene))),
		m_pipeline(new Pipeline(graphicsStage, PIPELINE_CREATE))
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
