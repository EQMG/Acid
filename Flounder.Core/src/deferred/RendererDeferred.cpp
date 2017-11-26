#include "RendererDeferred.hpp"

#include "../camera/Camera.hpp"
#include "../devices/Display.hpp"
#include "../entities/components/ComponentLight.hpp"
#include "../entities/Entities.hpp"
#include "../models/Vertex.hpp"
#include "../renderer/Renderer.hpp"
#include "../shadows/Shadows.hpp"
#include "../textures/Texture.hpp"
#include "../worlds/Worlds.hpp"
#include "UbosDeferred.hpp"

namespace Flounder
{
	const std::vector<Vertex> VERTICES = 
	{
		Vertex(Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f))
	};
	const std::vector<uint32_t> INDICES = 
	{
		0, 1, 2, 2, 3, 0
	};

	const DescriptorType RendererDeferred::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerDepth = DepthStencil::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerColour = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerNormal = Texture::CreateDescriptor(3, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerMaterial = Texture::CreateDescriptor(4, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerShadows = Texture::CreateDescriptor(5, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererDeferred::pipelineCreateInfo =
	{
		PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_NONE, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeSamplerDepth, typeSamplerColour, typeSamplerNormal, typeSamplerMaterial, typeSamplerShadows }, // descriptors

		{ "res/shaders/deferred/deferred.vert.spv", "res/shaders/deferred/deferred.frag.spv" } // shaderStages
	};

	RendererDeferred::RendererDeferred(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosDeferred::UboScene))),
		m_pipeline(new Pipeline("deferred", pipelineCreateInfo, subpass)),
		m_model(new Model(VERTICES, INDICES))
	{
	}

	RendererDeferred::~RendererDeferred()
	{
		delete m_uniformScene;
		delete m_pipeline;
		delete m_model;
	}

	void RendererDeferred::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosDeferred::UboScene uboScene = {};

		int lightsLoaded = 0;

		for (auto entity : *Entities::Get()->GetEntities())
		{
			ComponentLight *componentLight = entity->GetComponent<ComponentLight*>();

			if (componentLight != nullptr && lightsLoaded < NUMBER_LIGHTS)
			{
				uboScene.lightColours[lightsLoaded] = Colour(*componentLight->GetLight()->m_colour);
				uboScene.lightPositions[lightsLoaded] = Vector3(*componentLight->GetLight()->m_position);
				uboScene.lightAttenuations[lightsLoaded] = Attenuation(*componentLight->GetLight()->m_attenuation);
				lightsLoaded++;
			}
		}

		for (int i = lightsLoaded; i < NUMBER_LIGHTS; i++)
		{
			uboScene.lightColours[i] = Colour(0.0f, 0.0f, 0.0f, 0.0f);
			uboScene.lightPositions[i] = Vector3();
			uboScene.lightAttenuations[i] = Attenuation();
		}

		uboScene.projection = Matrix4(*camera.GetProjectionMatrix());
		uboScene.view = Matrix4(*camera.GetViewMatrix());
		uboScene.shadowSpace = Matrix4(*Shadows::Get()->GetShadowBox()->GetToShadowMapSpaceMatrix());

		uboScene.fogColour = Colour(*Skyboxes::Get()->GetFog()->m_colour);
		uboScene.fogDensity = Skyboxes::Get()->GetFog()->m_density;
		uboScene.fogGradient = Skyboxes::Get()->GetFog()->m_gradient;

		uboScene.shadowDistance = Shadows::Get()->GetShadowBoxDistance();
		uboScene.shadowTransition = Shadows::Get()->GetShadowTransition();
		uboScene.shadowBias = Shadows::Get()->GetShadowBias();
		uboScene.shadowDarkness = Shadows::Get()->GetShadowDarkness();
		uboScene.shadowMapSize = Shadows::Get()->GetShadowSize();
		uboScene.shadowPCF = Shadows::Get()->GetShadowPcf();

		m_uniformScene->Update(&uboScene);

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ 
			m_uniformScene->GetWriteDescriptor(0, descriptorSet), 
			Renderer::Get()->GetDepthStencil()->GetWriteDescriptor(1, descriptorSet),
			Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(2, descriptorSet),
			Renderer::Get()->GetSwapchain()->GetNormalImage()->GetWriteDescriptor(3, descriptorSet),
			Renderer::Get()->GetSwapchain()->GetMaterialImage()->GetWriteDescriptor(4, descriptorSet),
			Renderer::Get()->GetSwapchain()->GetShadowImage()->GetWriteDescriptor(5, descriptorSet),
		};
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		
		VkDescriptorSet descriptors[1] = { descriptorSet };
		vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(*commandBuffer);
	}
}
