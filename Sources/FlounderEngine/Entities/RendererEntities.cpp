#include <Devices/Display.hpp>
#include "RendererEntities.hpp"

#include "../Renderer/Renderer.hpp"
#include "../Models/Model.hpp"
#include "../Scenes/Scenes.hpp"
#include "UbosEntities.hpp"
#include "EntityRender.hpp"

namespace Flounder
{
	const PipelineCreate PIPELINE_CREATE =
		{
			PIPELINE_MRT, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_BACK_BIT, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(2), // vertexAttributeDescriptions

			{
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT), // uboScene
				UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL), // uboObject
				Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerDiffuse
				Texture::CreateDescriptor(3, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerMaterial
			}, // descriptors

			{"Resources/Shaders/Entities/Entity.vert.spv", "Resources/Shaders/Entities/Entity.frag.spv"} // shaderStages
		};

	RendererEntities::RendererEntities(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosEntities::UboScene))),
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE))
	{
	}

	RendererEntities::~RendererEntities()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererEntities::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosEntities::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<EntityRender *> renderList = std::vector<EntityRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<EntityRender>(&renderList);

		for (auto entityRender : renderList)
		{
			entityRender->CmdRender(commandBuffer, *m_pipeline, *m_uniformScene);
		}
	}
}
