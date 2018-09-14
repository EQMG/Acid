#include "RendererDeferred.hpp"

#include "Helpers/FileSystem.hpp"
#include "Lights/Light.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Models/VertexModel.hpp"
#include "Renderer/Swapchain/DepthStencil.hpp"
#include "Renderer/Pipelines/Compute.hpp"
#include "Scenes/Scenes.hpp"
#include "Shadows/Shadows.hpp"
#include "Skyboxes/MaterialSkybox.hpp"

namespace acid
{
	const uint32_t RendererDeferred::MAX_LIGHTS = 32;

	struct DeferredLight
	{
		Colour m_colour;
		Vector3 m_position;
		float m_radius;
	};

	RendererDeferred::RendererDeferred(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_descriptorSet(DescriptorsHandler()),
		m_uniformScene(UniformHandler()),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Deferred/Deferred.vert", "Shaders/Deferred/Deferred.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_NONE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, GetDefines()))),
		m_model(ModelRectangle::Resource(-1.0f, 1.0f)),
		m_brdf(ComputeBrdf(512)),
		m_fog(Fog(Colour::WHITE, 0.001f, 2.0f, -0.1f, 0.3f))
	{
	}

	void RendererDeferred::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		auto sceneSkyboxRender = Scenes::Get()->GetStructure()->GetComponent<MaterialSkybox>();
		auto ibl = (sceneSkyboxRender == nullptr) ? nullptr : sceneSkyboxRender->GetCubemap(); // TODO: IBL cubemap.

		// Updates uniforms.
		auto deferredLights = std::vector<DeferredLight>(MAX_LIGHTS);
		uint32_t lightCount = 0;

		auto sceneLights = Scenes::Get()->GetStructure()->QueryComponents<Light>();

		for (auto &light : sceneLights)
		{
		//	auto position = *light->GetPosition();
		//	float radius = light->GetRadius();

		//	if (radius >= 0.0f && !camera.GetViewFrustum()->SphereInFrustum(position, radius))
		//	{
		//		continue;
		//	}

			DeferredLight deferredLight = {};
			deferredLight.m_colour = light->GetColour();
			deferredLight.m_position = light->GetPosition();
			deferredLight.m_radius = light->GetRadius();
			deferredLights[lightCount] = deferredLight;
			lightCount++;

			if (lightCount >= MAX_LIGHTS)
			{
				break;
			}
		}

		// Updates uniforms.
		m_uniformScene.Push("lights", *deferredLights.data(), sizeof(DeferredLight) * MAX_LIGHTS);
		m_uniformScene.Push("lightsCount", lightCount);
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());
		m_uniformScene.Push("shadowSpace", Shadows::Get()->GetShadowBox().GetToShadowMapSpaceMatrix());
		m_uniformScene.Push("fogColour", m_fog.GetColour());
		m_uniformScene.Push("cameraPosition", camera.GetPosition());
		m_uniformScene.Push("fogDensity", m_fog.GetDensity());
		m_uniformScene.Push("fogGradient", m_fog.GetGradient());
		m_uniformScene.Push("shadowDistance", Shadows::Get()->GetShadowBoxDistance());
		m_uniformScene.Push("shadowTransition", Shadows::Get()->GetShadowTransition());
		m_uniformScene.Push("shadowBias", Shadows::Get()->GetShadowBias());
		m_uniformScene.Push("shadowDarkness", Shadows::Get()->GetShadowDarkness());
		m_uniformScene.Push("shadowPCF", Shadows::Get()->GetShadowPcf());

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", m_uniformScene);
		m_descriptorSet.Push("samplerDepth", m_pipeline.GetDepthStencil());
		m_descriptorSet.Push("samplerDiffuse", m_pipeline.GetTexture(2));
		m_descriptorSet.Push("samplerNormal", m_pipeline.GetTexture(3));
		m_descriptorSet.Push("samplerMaterial", m_pipeline.GetTexture(4));
		m_descriptorSet.Push("samplerShadows", m_pipeline.GetTexture(0, 0));
		m_descriptorSet.Push("samplerBrdf", m_brdf);
		m_descriptorSet.Push("samplerIbl", ibl);
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

	std::vector<PipelineDefine> RendererDeferred::GetDefines()
	{
		std::vector<PipelineDefine> result = {};
		result.emplace_back(PipelineDefine("USE_IBL", "TRUE"));
		result.emplace_back(PipelineDefine("MAX_LIGHTS", String::To(MAX_LIGHTS)));
		return result;
	}

	std::shared_ptr<Texture> RendererDeferred::ComputeBrdf(const uint32_t &size)
	{
		auto result = std::make_shared<Texture>(size, size);

		// Creates the pipeline.
		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
		Compute compute = Compute(ComputeCreate("Shaders/Brdf.comp", size, size, 16, {}));

		// Bind the pipeline.
		compute.BindPipeline(commandBuffer);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
		descriptorSet.Push("outColour", result);
		descriptorSet.Update(compute);

		// Runs the compute pipeline.
		descriptorSet.BindDescriptor(commandBuffer);
		compute.CmdRender(commandBuffer);
		commandBuffer.End();
		commandBuffer.Submit();

#if ACID_VERBOSE
		// Saves the brdf texture.
		std::string filename = FileSystem::GetWorkingDirectory() + "/Brdf.png";
		FileSystem::ClearFile(filename);
		std::unique_ptr<uint8_t[]> pixels(result->GetPixels());
		Texture::WritePixels(filename, pixels.get(), result->GetWidth(), result->GetHeight(), result->GetComponents());
#endif

		return result;
	}
}
