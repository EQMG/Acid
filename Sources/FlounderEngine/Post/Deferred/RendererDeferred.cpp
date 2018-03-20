#include "RendererDeferred.hpp"

#include "../../Scenes/Scenes.hpp"
#include "../../Devices/Display.hpp"
#include "../../Lights/Light.hpp"
#include "../../Models/Shapes/ShapeRectangle.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../Shadows/Shadows.hpp"
#include "../../Worlds/Worlds.hpp"
#include "UbosDeferred.hpp"

namespace Flounder
{
	const PipelineCreate PIPELINE_CREATE =
	{
		PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT, // cullModeFlags

		VertexModel::GetBindingDescriptions(), // vertexBindingDescriptions
		VertexModel::GetAttributeDescriptions(1), // vertexAttributeDescriptions
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
		m_descriptorSet(nullptr),
		m_pipeline(new Pipeline(graphicsStage, PIPELINE_CREATE)),
		m_model(ShapeRectangle::Resource(-1.0f, 1.0f))
	{
	}

	RendererDeferred::~RendererDeferred()
	{
		delete m_uniformScene;
		delete m_descriptorSet;
		delete m_pipeline;
		delete m_model;
	}

	void RendererDeferred::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(*m_pipeline);
		}

		m_descriptorSet->Update({
			m_uniformScene,
			m_pipeline->GetTexture(2),
			m_pipeline->GetDepthStencil(),
			m_pipeline->GetTexture(2),
			m_pipeline->GetTexture(3),
			m_pipeline->GetTexture(4),
			m_pipeline->GetTexture(0, 0)
		});

		// Updates uniforms.
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

		uboScene.fogColour = *Worlds::Get()->GetFog()->m_colour;
		uboScene.cameraPosition = *Scenes::Get()->GetCamera()->GetPosition();
		uboScene.fogDensity = Worlds::Get()->GetFog()->m_density;
		uboScene.fogGradient = Worlds::Get()->GetFog()->m_gradient;

		uboScene.shadowDistance = Shadows::Get()->GetShadowBoxDistance();
		uboScene.shadowTransition = Shadows::Get()->GetShadowTransition();
		uboScene.shadowBias = Shadows::Get()->GetShadowBias();
		uboScene.shadowDarkness = Shadows::Get()->GetShadowDarkness();
		uboScene.shadowPCF = Shadows::Get()->GetShadowPcf();

		uboScene.lightsCount = static_cast<int>(sceneLights.size());
		m_uniformScene->Update(&uboScene);

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
