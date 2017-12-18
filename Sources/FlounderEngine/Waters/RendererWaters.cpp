#include "RendererWaters.hpp"

#include "../Textures/Texture.hpp"
#include "UbosWaters.hpp"

namespace Flounder
{
	const DescriptorType RendererWaters::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererWaters::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL);
	const DescriptorType RendererWaters::typeSamplerReflection = Texture::CreateDescriptor(3, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererWaters::pipelineCreateInfo =
	{
		PIPELINE_MRT, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_NONE, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeUboObject, typeSamplerReflection }, // descriptors

		{ "Resources/Shaders/Waters/Water.vert.spv", "Resources/Shaders/Waters/Water.frag.spv" } // shaderStages
	};

	RendererWaters::RendererWaters(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosWaters::UboScene))),
		m_pipeline(new Pipeline(pipelineCreateInfo, subpass))
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

		if (Waters::Get()->GetWater() != nullptr)
		{
			Waters::Get()->GetWater()->CmdRender(commandBuffer, *m_pipeline, *m_uniformScene);
		}
	}
}
