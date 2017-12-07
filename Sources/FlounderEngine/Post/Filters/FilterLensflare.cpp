#include "FilterLensflare.hpp"

#include "../../Camera/Camera.hpp"
#include "../../Devices/Display.hpp"
#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const DescriptorType FilterLensflare::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType FilterLensflare::typeSamplerColour = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType FilterLensflare::typeSamplerMaterial = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);

	FilterLensflare::FilterLensflare(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Lensflare.frag.spv", subpass, { typeUboScene, typeSamplerColour, typeSamplerMaterial }),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_sunPosition(new Vector3()),
		m_sunHeight(0.0f)
	{
	}

	FilterLensflare::~FilterLensflare()
	{
		delete m_uniformScene;
		delete m_sunPosition;
	}

	void FilterLensflare::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
		UboScene uboScene = {};
		uboScene.sunPosition = *m_sunPosition;
		uboScene.worldHeight = m_sunHeight;
		uboScene.displaySize = Vector2(static_cast<float>(Display::Get()->GetWidth()), static_cast<float>(Display::Get()->GetHeight()));
		m_uniformScene->Update(&uboScene);

		const auto descriptorSet = m_pipeline->GetDescriptorSet();
		const std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
		{
			m_uniformScene->GetWriteDescriptor(0, descriptorSet),
			Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(1, descriptorSet),
			Renderer::Get()->GetSwapchain()->GetMaterialImage()->GetWriteDescriptor(2, descriptorSet)
		};
		IPostFilter::CmdRender(commandBuffer, descriptorWrites);
	}

	void FilterLensflare::SetSunPosition(const Vector3 &sunPosition) const
	{
		ICamera *camera = Camera::Get()->GetCamera();
		Matrix4::WorldToScreenSpace(sunPosition, *camera->GetViewMatrix(), *camera->GetProjectionMatrix(), m_sunPosition);
	}
}
