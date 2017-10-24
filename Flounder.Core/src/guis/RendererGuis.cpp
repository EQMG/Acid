#include "RendererGuis.hpp"

#include "../devices/Display.hpp"
#include "../uis/Uis.hpp"
#include "UbosGuis.hpp"

namespace Flounder
{
	const DescriptorType RendererGuis::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	const DescriptorType RendererGuis::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL);
	const DescriptorType RendererGuis::typeSamplerTexture = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererGuis::pipelineCreateInfo =
	{
		PIPELINE_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_NONE, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeUboObject, typeSamplerTexture }, // descriptors

		{ "res/shaders/guis/gui.vert.spv", "res/shaders/guis/gui.frag.spv" } // shaderStages
	};

	RendererGuis::RendererGuis() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosGuis::UboScene))),
		m_pipeline(new Pipeline("guis", pipelineCreateInfo))
	{
	}

	RendererGuis::~RendererGuis()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererGuis::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosGuis::UboScene uboScene = {};
		uboScene.aspectRatio = static_cast<float>(Display::Get()->GetAspectRatio());
		m_uniformScene->Update(&uboScene);

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		for (auto screenobject : *Uis::Get()->GetObjects())
		{
			Gui *object = dynamic_cast<Gui*>(screenobject);

			if (object != nullptr)
			{
				object->CmdRender(*commandBuffer, *m_pipeline, *m_uniformScene);
			}
		}
	}
}
