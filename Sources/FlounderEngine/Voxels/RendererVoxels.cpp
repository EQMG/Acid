#include <Devices/Display.hpp>
#include "RendererVoxels.hpp"

#include "../Renderer/Renderer.hpp"
#include "../Models/Model.hpp"
#include "../Scenes/Scenes.hpp"
#include "UbosVoxels.hpp"
#include "VoxelRender.hpp"

namespace Flounder
{
	RendererVoxels::RendererVoxels(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosVoxels::UboScene))),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Voxels/Voxel.vert", "Resources/Shaders/Voxels/Voxel.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), { }))
	{
	}

	RendererVoxels::~RendererVoxels()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererVoxels::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosVoxels::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<VoxelRender *> renderList = std::vector<VoxelRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<VoxelRender>(&renderList);

		for (auto entityRender : renderList)
		{
			entityRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
