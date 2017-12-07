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
		PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboObject, typeSamplerTexture }, // descriptors

		{ "Resources/Shaders/Guis/Gui.vert.spv", "Resources/Shaders/Guis/Gui.frag.spv" } // shaderStages
	};

	RendererGuis::RendererGuis(const int &subpass) :
		IRenderer(),
		m_pipeline(new Pipeline(pipelineCreateInfo, subpass))
	{
	}

	RendererGuis::~RendererGuis()
	{
		delete m_pipeline;
	}

	void RendererGuis::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_pipeline->BindPipeline(commandBuffer);

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
