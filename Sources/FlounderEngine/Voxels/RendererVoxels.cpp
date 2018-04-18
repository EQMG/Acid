#include "RendererVoxels.hpp"

#include "Renderer/Renderer.hpp"
#include "Models/Model.hpp"
#include "Scenes/Scenes.hpp"
#include "VoxelRender.hpp"

namespace Flounder
{
	RendererVoxels::RendererVoxels(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Voxels/Voxel.vert", "Resources/Shaders/Voxels/Voxel.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), {})),
		m_uniformScene(new UniformHandler())
	{
	}

	RendererVoxels::~RendererVoxels()
	{
		delete m_pipeline;
		delete m_uniformScene;
	}

	void RendererVoxels::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene->Push("projection", *camera.GetProjectionMatrix());
		m_uniformScene->Push("view", *camera.GetViewMatrix());

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<VoxelRender *> renderList = std::vector<VoxelRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<VoxelRender>(&renderList);

		for (auto entityRender : renderList)
		{
			entityRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
