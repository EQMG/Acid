#include "RendererGuis.hpp"

#include "../Models/Vertex.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Uis/Uis.hpp"
#include "Gui.hpp"

namespace Flounder
{
	const PipelineCreate PIPELINE_CREATE =
		{
			PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_BACK_BIT, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(1), // vertexAttributeDescriptions

			{
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_ALL), // uboObject
				Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerTexture
			}, // descriptors

			{"Resources/Shaders/Guis/Gui.vert.spv", "Resources/Shaders/Guis/Gui.frag.spv"} // shaderStages
		};

	RendererGuis::RendererGuis(const int &subpass) :
		IRenderer(),
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE))
	{
	}

	RendererGuis::~RendererGuis()
	{
		delete m_pipeline;
	}

	void RendererGuis::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_pipeline->BindPipeline(commandBuffer);

		for (auto screenObject : *Uis::Get()->GetObjects())
		{
			if (!screenObject->IsVisible())
			{
				continue;
			}

			Gui *object = dynamic_cast<Gui *>(screenObject);

			if (object != nullptr)
			{
				object->CmdRender(commandBuffer, *m_pipeline);
			}
		}
	}
}
