#include "RendererFonts.hpp"

namespace Flounder
{
	const PipelineCreateInfo PIPELINE_CREATE_INFO =
		{
			PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_BACK_BIT, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(1), // vertexAttributeDescriptions

			{
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_ALL), // uboObject
				Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT) // amplerTexture
			}, // descriptors

			{"Resources/Shaders/Fonts/Font.vert.spv", "Resources/Shaders/Fonts/Font.frag.spv"} // shaderStages
		};

	RendererFonts::RendererFonts(const int &subpass) :
		IRenderer(),
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE_INFO))
	{
	}

	RendererFonts::~RendererFonts()
	{
		delete m_pipeline;
	}

	void RendererFonts::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_pipeline->BindPipeline(commandBuffer);

		for (auto screenObject : *Uis::Get()->GetObjects())
		{
			Text *object = dynamic_cast<Text *>(screenObject);

			if (object != nullptr)
			{
				object->CmdRender(commandBuffer, *m_pipeline);
			}
		}
	}
}
