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
			Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT), // writeColour
			DepthStencil::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerDepth
			Texture::CreateDescriptor(3, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerColour
			Texture::CreateDescriptor(4, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerNormal
			Texture::CreateDescriptor(5, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerMaterial
			Texture::CreateDescriptor(6, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerShadows
		}, // descriptors

		{
			"Resources/Shaders/Deferred/Deferred.vert.spv", "Resources/Shaders/Deferred/Deferred.frag.spv"
		} // shaderStages
	};

	RendererDeferred::RendererDeferred(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosDeferred::UboScene))),
		//m_uniformLights(new UniformBuffer(sizeof(UbosDeferred::UboLights))),
		m_pipeline(new Pipeline(graphicsStage, PIPELINE_CREATE)),
		m_model(ShapeRectangle::Resource(-1.0f, 1.0f))
	{
	}

	RendererDeferred::~RendererDeferred()
	{
		delete m_uniformScene;
		//delete m_uniformLights;
		delete m_pipeline;
		delete m_model;
	}

	void RendererDeferred::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		std::vector<UbosDeferred::Light> sceneLights = {};

		for (auto entity : *Scenes::Get()->GetStructure()->GetAll())
		{
			auto light = entity->GetComponent<Light>();

			if (light != nullptr)
			{
			//	auto position = *light->GetPosition();
			//	float radius = light->GetRadius();

			//	if (radius >= 0.0f && !camera.GetViewFrustum()->SphereInFrustum(position, radius))
			//	{
			//		continue;
			//	}

				if (light->GetColour()->LengthSquared() == 0.0f)
				{
					continue;
				}

				UbosDeferred::Light lightObject = {};
				lightObject.colour = *light->GetColour();
				lightObject.position = *light->GetPosition();
				lightObject.radius = light->GetRadius();
				sceneLights.push_back(lightObject);
			}

			if (sceneLights.size() >= MAX_LIGHTS)
			{
				break;
			}
		}

	//	printf("Rendered Lights: %i\n", sceneLights.size());

		UbosDeferred::UboScene uboScene = {};

		for (unsigned int i = 0; i < sceneLights.size(); i++)
		{
			uboScene.lights[i] = sceneLights.at(i);
		}

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

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
		{
			m_uniformScene->GetWriteDescriptor(0, descriptorSet),
			m_pipeline->GetTexture(2)->GetWriteDescriptor(1, descriptorSet),
			m_pipeline->GetDepthStencil()->GetWriteDescriptor(2, descriptorSet),
			m_pipeline->GetTexture(2)->GetWriteDescriptor(3, descriptorSet),
			m_pipeline->GetTexture(3)->GetWriteDescriptor(4, descriptorSet),
			m_pipeline->GetTexture(4)->GetWriteDescriptor(5, descriptorSet),
			m_pipeline->GetTexture(0, 0)->GetWriteDescriptor(6, descriptorSet)
		};
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[] = {descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(commandBuffer);
	}
}
