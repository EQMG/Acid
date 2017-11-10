#include "RendererFonts.hpp"

#include "../devices/Display.hpp"
#include "../uis/Uis.hpp"
#include "UbosFonts.hpp"

namespace Flounder
{
	const DescriptorType RendererFonts::typeUboObject = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_ALL);
	const DescriptorType RendererFonts::typeSamplerTexture = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererFonts::pipelineCreateInfo =
	{
		PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_NONE, // cullModeFlags
		2, // subpass

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboObject, typeSamplerTexture }, // descriptors

		{ "res/shaders/fonts/font.vert.spv", "res/shaders/fonts/font.frag.spv" } // shaderStages
	};

	RendererFonts::RendererFonts() :
		IRenderer(),
		m_pipeline(new Pipeline("fonts", pipelineCreateInfo))
	{
	}

	RendererFonts::~RendererFonts()
	{
		delete m_pipeline;
	}

	void RendererFonts::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		for (auto screenobject : *Uis::Get()->GetObjects())
		{
			Text *object = dynamic_cast<Text*>(screenobject);

			if (object != nullptr)
			{
				object->CmdRender(*commandBuffer, *m_pipeline);
			}
		}
	}
}
