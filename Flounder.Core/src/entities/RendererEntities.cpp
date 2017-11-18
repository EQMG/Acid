#include "RendererEntities.hpp"

#include "../devices/Display.hpp"
#include "../camera/Camera.hpp"
#include "../maths/Maths.hpp"
#include "../renderer/Renderer.hpp"
#include "../terrains/Terrains.hpp"
#include "UbosEntities.hpp"
#include "Entities.hpp"

namespace Flounder
{
	const DescriptorType RendererEntities::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererEntities::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererEntities::typeSamplerDiffuse = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererEntities::typeSamplerSway = Texture::CreateDescriptor(3, VK_SHADER_STAGE_VERTEX_BIT);
	const PipelineCreateInfo RendererEntities::pipelineCreateInfo =
	{
		PIPELINE_MRT, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeUboObject, typeSamplerDiffuse, typeSamplerSway }, // descriptors

		{ "res/shaders/entities/entity.vert.spv", "res/shaders/entities/entity.frag.spv" } // shaderStages
	};

	RendererEntities::RendererEntities(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosEntities::UboScene))),
		m_pipeline(new Pipeline("entities", pipelineCreateInfo, subpass))
	{
	}

	RendererEntities::~RendererEntities()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererEntities::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosEntities::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		for (auto entity : *Entities::Get()->GetEntities()) // Entities::Get()->GetStructure()->GetAll()
		{
			entity->CmdRender(*commandBuffer, *m_pipeline, *m_uniformScene);
		}
	}
}
