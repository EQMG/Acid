#include "FilterWobble.hpp"

#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
		{
			UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // uboScene
			Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT), // writeColour
			Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerColour
		};

	FilterWobble::FilterWobble(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Wobble.frag.spv", graphicsStage, DESCRIPTORS),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_wobbleSpeed(2.0f),
		m_wobbleAmount(0.0f)
	{
	}

	FilterWobble::~FilterWobble()
	{
		delete m_uniformScene;
	}

	void FilterWobble::Render(const VkCommandBuffer &commandBuffer)
	{
		m_wobbleAmount += m_wobbleSpeed * Engine::Get()->GetDeltaRender();

		UboScene uboScene = {};
		uboScene.moveIt = m_wobbleAmount;
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
