#include "RendererFonts.hpp"

#include "../Devices/Display.hpp"
#include "../Uis/Uis.hpp"
#include "UbosFonts.hpp"

namespace Flounder
{
	const DescriptorType RendererFonts::typeUboObject = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_ALL);
	const DescriptorType RendererFonts::typeSamplerTexture = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererFonts::pipelineCreateInfo =
	{
		PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboObject, typeSamplerTexture }, // descriptors

		{ "Resources/Shaders/Fonts/Font.vert.spv", "Resources/Shaders/Fonts/Font.frag.spv" } // shaderStages
	};

	RendererFonts::RendererFonts(const int &subpass) :
		IRenderer(),
		m_pipeline(new Pipeline(pipelineCreateInfo, subpass))
	{
	}

	RendererFonts::~RendererFonts()
	{
		delete m_pipeline;
	}

	void RendererFonts::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_pipeline->BindPipeline(commandBuffer);

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
