#include "RendererFonts.hpp"

#include "../devices/Display.hpp"
#include "../uis/Uis.hpp"
#include "UbosFonts.hpp"

namespace Flounder
{
	const DescriptorType RendererFonts::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererFonts::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL);
	const DescriptorType RendererFonts::typeSamplerTexture = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererFonts::pipelineCreateInfo =
	{
		PIPELINE_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_NONE, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeUboObject, typeSamplerTexture }, // descriptors

		{ "res/shaders/fonts/font.vert.spv", "res/shaders/fonts/font.frag.spv" } // shaderStages
	};

	RendererFonts::RendererFonts() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosFonts::UboScene))),
		m_pipeline(new Pipeline("fonts", pipelineCreateInfo))
	{
	}

	RendererFonts::~RendererFonts()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererFonts::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosFonts::UboScene uboScene = {};
		uboScene.aspectRatio = static_cast<float>(Display::Get()->GetAspectRatio());
		m_uniformScene->Update(&uboScene);

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		for (auto screenobject : *Uis::Get()->GetObjects())
		{
			Text *object = dynamic_cast<Text*>(screenobject);

			if (object != nullptr)
			{
				object->CmdRender(*commandBuffer, *m_pipeline, *m_uniformScene);
			}
		}
	}
}
