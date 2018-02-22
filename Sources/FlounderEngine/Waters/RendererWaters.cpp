#include "RendererWaters.hpp"

#include "../Scenes/Scenes.hpp"
#include "../Models/Model.hpp"
#include "../Textures/Texture.hpp"
#include "UbosWaters.hpp"

namespace Flounder
{
	const PipelineCreateInfo PIPELINE_CREATE_INFO =
		{
			PIPELINE_MRT, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_NONE, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(2), // vertexAttributeDescriptions

			{
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT), // uboScene
				UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL), // uboObject
				Texture::CreateDescriptor(3, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerReflection
			}, // descriptors

			{"Resources/Shaders/Waters/Water.vert.spv", "Resources/Shaders/Waters/Water.frag.spv"} // shaderStages
		};

	RendererWaters::RendererWaters(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosWaters::UboScene))),
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE_INFO))
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

		for (auto object : *Scenes::Get()->GetStructure()->GetAll())
		{
			auto waterRender = object->GetComponent<WaterRender>();

			if (waterRender != nullptr)
			{
				waterRender->CmdRender(commandBuffer, *m_pipeline, *m_uniformScene);
			}
		}
	}
}
