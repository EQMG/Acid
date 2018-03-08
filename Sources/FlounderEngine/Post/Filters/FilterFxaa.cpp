#include "FilterFxaa.hpp"

#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
		{
			UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // uboScene
			Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT), // writeColour
			Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerColour
		};

	FilterFxaa::FilterFxaa(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Fxaa.frag.spv", graphicsStage, DESCRIPTORS),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_spanMax(8.0f)
	{
	}

	FilterFxaa::~FilterFxaa()
	{
		delete m_uniformScene;
	}

	void FilterFxaa::Render(const VkCommandBuffer &commandBuffer)
	{
		UboScene uboScene = {};
		uboScene.spanMax = m_spanMax;
		m_uniformScene->Update(&uboScene);

		/*const auto descriptorSet = m_pipeline->GetDescriptorSet();
		const std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
			{
				m_uniformScene->GetWriteDescriptor(0, *descriptorSet),
				m_pipeline->GetTexture(2)->GetWriteDescriptor(1, *descriptorSet),
				m_pipeline->GetTexture(2)->GetWriteDescriptor(2, *descriptorSet)
			};*/
		IPostFilter::CmdRender(commandBuffer);
	}
}
