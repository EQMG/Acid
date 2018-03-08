#include "FilterGrain.hpp"

#include "../../Textures/Texture.hpp"
#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
		{
			UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // uboScene
			Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT), // writeColour
			Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerColour
		};

	FilterGrain::FilterGrain(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Grain.frag.spv", graphicsStage, DESCRIPTORS),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_strength(2.3f)
	{
	}

	FilterGrain::~FilterGrain()
	{
		delete m_uniformScene;
	}

	void FilterGrain::Render(const VkCommandBuffer &commandBuffer)
	{
		UboScene uboScene = {};
		uboScene.strength = m_strength;
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
