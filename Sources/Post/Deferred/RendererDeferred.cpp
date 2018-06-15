#include "RendererDeferred.hpp"

#include "Scenes/Scenes.hpp"
#include "Lights/Light.hpp"
#include "Models/Shapes/ShapeRectangle.hpp"
#include "Renderer/Renderer.hpp"
#include "Shadows/Shadows.hpp"
#include "Skyboxes/MaterialSkybox.hpp"
#include "Worlds/Worlds.hpp"

namespace fl
{
	const int RendererDeferred::MAX_LIGHTS = 64;

	struct DeferredLight // TODO: Replace struct with actual light object.
	{
		Colour colour;
		Vector3 position;
		float radius;
	};

	RendererDeferred::RendererDeferred(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_descriptorSet(DescriptorsHandler()),
		m_uniformScene(UniformHandler()),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Deferred/Deferred.vert", "Shaders/Deferred/Deferred.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON_NO_DEPTH, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK), {{"USE_IBL", "TRUE"}, {"MAX_LIGHTS", std::to_string(MAX_LIGHTS)}})),
		m_model(ShapeRectangle::Resource(-1.0f, 1.0f)),
		m_brdflut(Texture::Resource("BrdfLut.png"))
	{
	}

	RendererDeferred::~RendererDeferred()
	{
	}

	void RendererDeferred::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		auto skyboxRender = Scenes::Get()->GetStructure()->GetComponent<MaterialSkybox>();
		auto environment = (skyboxRender == nullptr) ? nullptr : skyboxRender->GetCubemap();

		// Updates uniforms.
		std::vector<DeferredLight> sceneLights = {};

		auto gameObjects = Scenes::Get()->GetStructure()->GetAll();

		for (auto &entity : gameObjects)
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

				if (light->GetColour().LengthSquared() == 0.0f)
				{
					continue;
				}

				DeferredLight lightObject = {};
				lightObject.colour = light->GetColour();
				lightObject.position = light->GetPosition();
				lightObject.radius = light->GetRadius();
				sceneLights.emplace_back(lightObject);
			}

			if (sceneLights.size() >= MAX_LIGHTS)
			{
				break;
			}
		}

		//	printf("Rendered Lights: %i\n", sceneLights.size());

		m_uniformScene.Push("lights", *sceneLights.data());
		m_uniformScene.Push("lightsCount", static_cast<int>(sceneLights.size()));
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());
		m_uniformScene.Push("shadowSpace", Shadows::Get()->GetShadowBox().GetToShadowMapSpaceMatrix());
		m_uniformScene.Push("fogColour", Worlds::Get()->GetWorld()->GetFog().GetColour());
		m_uniformScene.Push("cameraPosition", camera.GetPosition());
		m_uniformScene.Push("fogDensity", Worlds::Get()->GetWorld()->GetFog().GetDensity());
		m_uniformScene.Push("fogGradient", Worlds::Get()->GetWorld()->GetFog().GetGradient());
		m_uniformScene.Push("shadowDistance", Shadows::Get()->GetShadowBoxDistance());
		m_uniformScene.Push("shadowTransition", Shadows::Get()->GetShadowTransition());
		m_uniformScene.Push("shadowBias", Shadows::Get()->GetShadowBias());
		m_uniformScene.Push("shadowDarkness", Shadows::Get()->GetShadowDarkness());
		m_uniformScene.Push("shadowPCF", Shadows::Get()->GetShadowPcf());

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(2));
		m_descriptorSet.Push("samplerDepth", m_pipeline.GetDepthStencil());
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(2));
		m_descriptorSet.Push("samplerNormal", m_pipeline.GetTexture(3));
		m_descriptorSet.Push("samplerMaterial", m_pipeline.GetTexture(4));
		m_descriptorSet.Push("samplerShadows", m_pipeline.GetTexture(0, 0));
		m_descriptorSet.Push("samplerBrdflut", m_brdflut);
		m_descriptorSet.Push("samplerEnvironment", environment);
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
