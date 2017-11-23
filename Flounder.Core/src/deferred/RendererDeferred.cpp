#include "RendererDeferred.hpp"

#include "../devices/Display.hpp"
#include "../lights/Attenuation.hpp"
#include "../models/Vertex.hpp"
#include "../renderer/Renderer.hpp"
#include "../textures/Texture.hpp"
#include "../shadows/Shadows.hpp"
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
	const DescriptorType RendererDeferred::typeUboLights = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerColour = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerNormal = Texture::CreateDescriptor(3, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerShadows = Texture::CreateDescriptor(4, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererDeferred::pipelineCreateInfo =
	{
		PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_NONE, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeUboLights, typeSamplerColour, typeSamplerNormal, typeSamplerShadows }, // descriptors

		{ "res/shaders/deferred/deferred.vert.spv", "res/shaders/deferred/deferred.frag.spv" } // shaderStages
	};

	RendererDeferred::RendererDeferred(const int &subpass) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosDeferred::UboScene))),
		m_uniformLights(new UniformBuffer(sizeof(UbosDeferred::UboLights))),
		m_pipeline(new Pipeline("deferred", pipelineCreateInfo, subpass)),
		m_model(new Model(VERTICES, INDICES))
	{
	}

	RendererDeferred::~RendererDeferred()
	{
		delete m_uniformScene;
		delete m_uniformLights;
		delete m_pipeline;
		delete m_model;
	}

	void RendererDeferred::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosDeferred::UboScene uboScene = {};

		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
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

		UbosDeferred::UboLights uboLights = {};

		for (int i = 0; i < NUMBER_LIGHTS; i++)
		{
			// TODO: Load real lights.
			if (i == 0)
			{
				uboLights.lightColours[i] = Colour(1.0f, 1.0f, 1.0f, 1.0f); // *Worlds::Get()->GetSunColour()
				uboLights.lightPositions[i] = Vector3(0.0f, 100.0f, 100.0f); // *Worlds::Get()->GetSunPosition()
				uboLights.lightAttenuations[i] = Attenuation(1.0f, 0.0f, 0.0f);
			}
			else
			{
				uboLights.lightColours[i] = Colour(0.0f, 0.0f, 0.0f, 0.0f);
				uboLights.lightPositions[i] = Vector3();
				uboLights.lightAttenuations[i] = Attenuation();
			}
		}

		m_uniformLights->Update(&uboLights);

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ 
			m_uniformScene->GetWriteDescriptor(0, descriptorSet), 
			m_uniformLights->GetWriteDescriptor(1, descriptorSet),
			Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(2, descriptorSet), 
			Renderer::Get()->GetSwapchain()->GetNormalImage()->GetWriteDescriptor(3, descriptorSet), 
			Renderer::Get()->GetSwapchain()->GetShadowImage()->GetWriteDescriptor(4, descriptorSet) 
		};
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		
		VkDescriptorSet descriptors[1] = { m_pipeline->GetDescriptorSet() };
		vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(*commandBuffer);
	}
}
