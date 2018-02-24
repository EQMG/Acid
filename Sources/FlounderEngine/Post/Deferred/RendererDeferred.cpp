#include "RendererDeferred.hpp"

#include "../../Scenes/Scenes.hpp"
#include "../../Devices/Display.hpp"
#include "../../Lights/Light.hpp"
#include "../../Models/Shapes/ShapeRectangle.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../Shadows/Shadows.hpp"
#include "../../Skyboxes/Skyboxes.hpp"
#include "UbosDeferred.hpp"

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
				UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // uboScene
				UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT), // uboLights
				Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT), // writeColour
				DepthStencil::CreateDescriptor(3, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerDepth
				Texture::CreateDescriptor(4, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerColour
				Texture::CreateDescriptor(5, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerNormal
				Texture::CreateDescriptor(6, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerMaterial
				Texture::CreateDescriptor(7, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerShadows
			}, // descriptors

			{
				"Resources/Shaders/Deferred/Deferred.vert.spv", "Resources/Shaders/Deferred/Deferred.frag.spv"
			} // shaderStages
		};

	RendererDeferred::RendererDeferred(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosDeferred::UboScene))),
		m_uniformLights(new UniformBuffer(sizeof(UbosDeferred::UboLights))),
		m_pipeline(new Pipeline(subpass, PIPELINE_CREATE)),
		m_model(ShapeRectangle::Resource(-1.0f, 1.0f))
	{
	}

	RendererDeferred::~RendererDeferred()
	{
		delete m_uniformScene;
		delete m_uniformLights;
		delete m_pipeline;
		delete m_model;
	}

	void RendererDeferred::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		std::vector<UbosDeferred::Light> sceneLights = {};

		for (auto entity : *Scenes::Get()->GetStructure()->GetAll())
		{
			auto componentLight = entity->GetComponent<Light>();

			if (componentLight != nullptr)
			{
				//	Vector3 position = *componentLight->GetLight()->m_position;
				//	float radius = componentLight->GetLight()->m_radius;
				//	if (radius >= 0.0f && !camera.GetViewFrustum()->SphereInFrustum(position, radius))
				//	{
				//		continue;
				//	}

				UbosDeferred::Light light = {};
				light.colour = *componentLight->GetColour();
				light.position = *componentLight->GetPosition();
				light.radius = componentLight->GetRadius();
				sceneLights.push_back(light);
			}

			if (sceneLights.size() >= MAX_LIGHTS)
			{
				break;
			}
		}

		//	printf("Rendered Lights: %i\n", sceneLights.size());

		UbosDeferred::UboScene uboScene = {};

		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		uboScene.shadowSpace = *Shadows::Get()->GetShadowBox()->GetToShadowMapSpaceMatrix();

		uboScene.fogColour = *Skyboxes::Get()->GetFog()->m_colour;
		uboScene.cameraPosition = *Scenes::Get()->GetCamera()->GetPosition();
		uboScene.fogDensity = Skyboxes::Get()->GetFog()->m_density;
		uboScene.fogGradient = Skyboxes::Get()->GetFog()->m_gradient;

		uboScene.shadowDistance = Shadows::Get()->GetShadowBoxDistance();
		uboScene.shadowTransition = Shadows::Get()->GetShadowTransition();
		uboScene.shadowBias = Shadows::Get()->GetShadowBias();
		uboScene.shadowDarkness = Shadows::Get()->GetShadowDarkness();
		uboScene.shadowPCF = Shadows::Get()->GetShadowPcf();

		uboScene.lightsCount = static_cast<int>(sceneLights.size());

		m_uniformScene->Update(&uboScene);

		UbosDeferred::UboLights uboLights = {};

		for (unsigned int i = 0; i < sceneLights.size(); i++)
		{
			uboLights.lights[i] = sceneLights.at(i);
		}

		m_uniformLights->Update(&uboLights);

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
			{
				m_uniformScene->GetWriteDescriptor(0, descriptorSet),
				m_uniformLights->GetWriteDescriptor(1, descriptorSet),
				Renderer::Get()->GetDepthStencil()->GetWriteDescriptor(3, descriptorSet),
				Renderer::Get()->GetSwapchain()->GetTexture(2)->GetWriteDescriptor(2, descriptorSet),
				Renderer::Get()->GetSwapchain()->GetTexture(2)->GetWriteDescriptor(4, descriptorSet),
				Renderer::Get()->GetSwapchain()->GetTexture(3)->GetWriteDescriptor(5, descriptorSet),
				Renderer::Get()->GetSwapchain()->GetTexture(4)->GetWriteDescriptor(6, descriptorSet),
				Renderer::Get()->GetSwapchain()->GetTexture(5)->GetWriteDescriptor(7, descriptorSet)
			};
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[1] = {descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(commandBuffer);
	}
}
