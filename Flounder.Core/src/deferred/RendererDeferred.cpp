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
	const std::vector<Vertex> VERTICES = {
		Vertex(Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f))
	};
	const std::vector<uint32_t> INDICES = {
		0, 1, 2, 2, 3, 0
	};

	const DescriptorType RendererDeferred::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerColour = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerNormal = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerShadows = Texture::CreateDescriptor(3, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererDeferred::pipelineCreateInfo =
	{
		PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_NONE, // cullModeFlags
		2, // subpass

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeSamplerColour, typeSamplerNormal, typeSamplerShadows }, // descriptors

		{ "res/shaders/deferred/deferred.vert.spv", "res/shaders/deferred/deferred.frag.spv" } // shaderStages
	};

	RendererDeferred::RendererDeferred() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosDeferred::UboScene))),
		m_pipeline(new Pipeline("deferred", pipelineCreateInfo)),
		m_model(new Model(VERTICES, INDICES))
	{
	}

	RendererDeferred::~RendererDeferred()
	{
		delete m_model;
		delete m_pipeline;
	}

	void RendererDeferred::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosDeferred::UboScene uboScene = {};

		for (int i = 0; i < NUMBER_LIGHTS; i++)
		{
			UbosDeferred::Light light = {};

			// TODO: Load real lights.
			if (i == 0)
			{
				light.colour = Colour(*Worlds::Get()->GetSunColour());
				light.position = Vector3(*Worlds::Get()->GetSunPosition());
				light.attenuation = Attenuation();
				light.enabled = true;
			}
			else
			{
				light.colour = Colour();
				light.position = Vector3();
				light.attenuation = Attenuation();
				light.enabled = false;
			}

			uboScene.lights[i] = light;
		}

		uboScene.shadow = UbosDeferred::Shadow{};
		uboScene.shadow.shadowSpaceMatrix = Matrix4(*Shadows::Get()->GetShadowBox()->GetToShadowMapSpaceMatrix());
		uboScene.shadow.shadowDistance = Shadows::Get()->GetShadowBoxDistance();
		uboScene.shadow.shadowTransition = Shadows::Get()->GetShadowTransition();
		uboScene.shadow.shadowBias = Shadows::Get()->GetShadowBias();
		uboScene.shadow.shadowDarkness = Shadows::Get()->GetShadowDarkness();
		uboScene.shadow.shadowMapSize = Shadows::Get()->GetShadowSize();
		uboScene.shadow.shadowPCF = Shadows::Get()->GetShadowPcf();

		uboScene.fog = UbosDeferred::Fog{};
		uboScene.fog.colour = Colour(*Skyboxes::Get()->GetFog()->m_colour);
		uboScene.fog.density = Skyboxes::Get()->GetFog()->m_density;
		uboScene.fog.gradient = Skyboxes::Get()->GetFog()->m_gradient;

		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ m_uniformScene->GetWriteDescriptor(0, descriptorSet), Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(1, descriptorSet), Renderer::Get()->GetSwapchain()->GetNormalImage()->GetWriteDescriptor(2, descriptorSet), Renderer::Get()->GetSwapchain()->GetShadowImage()->GetWriteDescriptor(3, descriptorSet) };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		
		VkDescriptorSet descriptors[1] = { m_pipeline->GetDescriptorSet() };
		vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(*commandBuffer);
	}
}
