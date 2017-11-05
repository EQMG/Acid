#include "RendererGuis.hpp"

#include "../devices/Display.hpp"
#include "../models/Vertex.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../uis/Uis.hpp"
#include "Gui.hpp"
#include "UbosGuis.hpp"

namespace Flounder
{
	const DescriptorType RendererGuis::typeUboObject = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_ALL);
	const DescriptorType RendererGuis::typeSamplerTexture = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererGuis::pipelineCreateInfo =
	{
		PIPELINE_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_NONE, // cullModeFlags
		0, // subpass

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboObject, typeSamplerTexture }, // descriptors

		{ "res/shaders/guis/gui.vert.spv", "res/shaders/guis/gui.frag.spv" } // shaderStages
	};

	RendererGuis::RendererGuis() :
		IRenderer(),
		m_pipeline(new Pipeline("guis", pipelineCreateInfo))
	{
	}

	RendererGuis::~RendererGuis()
	{
		delete m_pipeline;
	}

	void RendererGuis::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		for (auto screenobject : *Uis::Get()->GetObjects())
		{
			Gui *object = dynamic_cast<Gui*>(screenobject);

			if (object != nullptr)
			{
				object->CmdRender(*commandBuffer, *m_pipeline);
			}
		}
	}
}
