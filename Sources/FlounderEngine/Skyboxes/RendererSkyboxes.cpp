#include "RendererSkyboxes.hpp"

#include "../Scenes/Scenes.hpp"
#include "../Models/Model.hpp"
#include "UbosSkyboxes.hpp"
#include "SkyboxRender.hpp"

namespace Flounder
{
	const PipelineCreate PIPELINE_CREATE =
		{
			PIPELINE_MRT_NO_DEPTH, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_FRONT_BIT, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(0), // vertexAttributeDescriptions

			{
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT), // uboScene
				UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL), // uboObject
				Cubemap::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerCubemap
			}, // descriptors

			{"Resources/Shaders/Skyboxes/Skybox.vert.spv", "Resources/Shaders/Skyboxes/Skybox.frag.spv"} // shaderStages
		};

	RendererSkyboxes::RendererSkyboxes(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosSkyboxes::UboScene))),
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE))
	{
	}

	RendererSkyboxes::~RendererSkyboxes()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererSkyboxes::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosSkyboxes::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		uboScene.view.m_30 = 0.0f;
		uboScene.view.m_31 = 0.0f;
		uboScene.view.m_32 = 0.0f;
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<SkyboxRender *> renderList = std::vector<SkyboxRender *>();
		Scenes::Get()->GetStructure()->QueryComponents(&renderList);

		for (auto skyboxRender : renderList)
		{
			skyboxRender->CmdRender(commandBuffer, *m_pipeline, *m_uniformScene);
		}
	}
}
