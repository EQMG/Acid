#include "FilterTiltshift.hpp"

#include "../../Devices/Display.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../Textures/Texture.hpp"

namespace Flounder
{
	const DescriptorType FilterTiltshift::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType FilterTiltshift::typeSamplerColour = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);

	FilterTiltshift::FilterTiltshift(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Tiltshift.frag.spv", subpass, { typeUboScene, typeSamplerColour }),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_blurAmount(1.0f),
		m_centre(1.1f),
		m_stepSize(0.004f),
		m_steps(3.0f)
	{
	}

	FilterTiltshift::~FilterTiltshift()
	{
		delete m_uniformScene;
	}

	void FilterTiltshift::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
		UboScene uboScene = {};
		uboScene.blurAmount = m_blurAmount;
		uboScene.centre = m_centre;
		uboScene.stepSize = m_stepSize;
		uboScene.steps = m_steps;
		m_uniformScene->Update(&uboScene);

		const auto descriptorSet = m_pipeline->GetDescriptorSet();
		const std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
		{
			m_uniformScene->GetWriteDescriptor(0, descriptorSet),
			Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(1, descriptorSet)
		};
		IPostFilter::CmdRender(commandBuffer, descriptorWrites);
	}
}
