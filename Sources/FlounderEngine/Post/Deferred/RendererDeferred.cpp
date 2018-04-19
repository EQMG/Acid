#include "RendererDeferred.hpp"

#include "Scenes/Scenes.hpp"
#include "Lights/Light.hpp"
#include "Models/Shapes/ShapeRectangle.hpp"
#include "Renderer/Renderer.hpp"
#include "Shadows/Shadows.hpp"
#include "Skyboxes/SkyboxRender.hpp"
#include "Worlds/Worlds.hpp"

namespace Flounder
{
#define MAX_LIGHTS 64

	struct DeferredLight
	{
		Colour colour;
		Vector3 position;
		float radius;
	};

	RendererDeferred::RendererDeferred(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_descriptorSet(new DescriptorsHandler()),
		m_uniformScene(new UniformHandler()),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Deferred/Deferred.vert", "Resources/Shaders/Deferred/Deferred.frag"},
			VertexModel::GetVertexInput(), PIPELINE_POLYGON_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), {{"USE_IBL", "TRUE"}, {"MAX_LIGHTS", std::to_string(MAX_LIGHTS)}})),
		m_model(ShapeRectangle::Resource(-1.0f, 1.0f)),
		m_brdflut(Texture::Resource("Resources/BrdfLut.png"))
	{
	}

	RendererDeferred::~RendererDeferred()
	{
		delete m_descriptorSet;
		delete m_uniformScene;
		delete m_pipeline;
		delete m_model;
	}

	void RendererDeferred::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		auto skyboxRender = Scenes::Get()->GetStructure()->GetComponent<SkyboxRender>();
		Cubemap *environment = (skyboxRender == nullptr) ? nullptr : skyboxRender->GetCubemap();

		// Updates uniforms.
		std::vector<DeferredLight> sceneLights = {};

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

				DeferredLight lightObject = {};
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

		m_uniformScene->Push("lights", *sceneLights.data());
		m_uniformScene->Push("lightsCount", static_cast<int>(sceneLights.size()));
		m_uniformScene->Push("projection", *camera.GetProjectionMatrix());
		m_uniformScene->Push("view", *camera.GetViewMatrix());
		m_uniformScene->Push("shadowSpace", *Shadows::Get()->GetShadowBox()->GetToShadowMapSpaceMatrix());
		m_uniformScene->Push("fogColour", *Worlds::Get()->GetFog()->m_colour);
		m_uniformScene->Push("cameraPosition", *Scenes::Get()->GetCamera()->GetPosition());
		m_uniformScene->Push("fogDensity", Worlds::Get()->GetFog()->m_density);
		m_uniformScene->Push("fogGradient", Worlds::Get()->GetFog()->m_gradient);
		m_uniformScene->Push("shadowDistance", Shadows::Get()->GetShadowBoxDistance());
		m_uniformScene->Push("shadowTransition", Shadows::Get()->GetShadowTransition());
		m_uniformScene->Push("shadowBias", Shadows::Get()->GetShadowBias());
		m_uniformScene->Push("shadowDarkness", Shadows::Get()->GetShadowDarkness());
		m_uniformScene->Push("shadowPCF", Shadows::Get()->GetShadowPcf());

		// Updates descriptors.
		m_descriptorSet->Push("UboScene", m_uniformScene);
		m_descriptorSet->Push("writeColour", m_pipeline->GetTexture(2));
		m_descriptorSet->Push("samplerDepth", m_pipeline->GetDepthStencil());
		m_descriptorSet->Push("samplerColour", m_pipeline->GetTexture(2));
		m_descriptorSet->Push("samplerNormal", m_pipeline->GetTexture(3));
		m_descriptorSet->Push("samplerMaterial", m_pipeline->GetTexture(4));
		m_descriptorSet->Push("samplerShadows", m_pipeline->GetTexture(0, 0));
		m_descriptorSet->Push("samplerBrdflut", m_brdflut);
		m_descriptorSet->Push("samplerEnvironment", environment);
		bool descriptorsSet = m_descriptorSet->Update(*m_pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
