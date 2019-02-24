#include "RendererDeferred.hpp"

#include "Files/FileSystem.hpp"
#include "Lights/Light.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Models/VertexModel.hpp"
#include "Renderer/Pipelines/PipelineCompute.hpp"
#include "Renderer/Renderer.hpp"
#include "Scenes/Scenes.hpp"
#include "Shadows/Shadows.hpp"
#include "Skyboxes/MaterialSkybox.hpp"

namespace acid
{
	static const uint32_t MAX_LIGHTS = 32; // TODO: Make configurable.

	RendererDeferred::RendererDeferred(const Pipeline::Stage &pipelineStage, const Type &type) :
		RenderPipeline(pipelineStage),
		m_type(type),
		m_pipeline(pipelineStage, {"Shaders/Deferred/Deferred.vert", "Shaders/Deferred/Deferred.frag"}, {VertexModel::GetVertexInput()},
			PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, false, GetDefines()),
		m_model(ModelRectangle::Create(-1.0f, 1.0f)),
		m_brdf(m_type == Type::Ibl ? ComputeBrdf(512) : nullptr),
		m_skybox(nullptr),
		m_ibl(nullptr),
		m_fog(Colour::White, 0.001f, 2.0f, -0.1f, 0.3f)
	{
	}

	void RendererDeferred::Render(const CommandBuffer &commandBuffer)
	{
		auto camera = Scenes::Get()->GetCamera();

		if (m_type == Type::Ibl)
		{
			auto materialSkybox = Scenes::Get()->GetStructure()->GetComponent<MaterialSkybox>();
			auto skybox = (materialSkybox == nullptr) ? nullptr : materialSkybox->GetCubemap();

			if (m_skybox != skybox)
			{
				m_skybox = skybox;
			//	m_ibl = ComputeIbl(m_skybox);
			}
		}

		// Updates uniforms.
		std::vector<DeferredLight> deferredLights(MAX_LIGHTS);
		uint32_t lightCount = 0;

		auto sceneLights = Scenes::Get()->GetStructure()->QueryComponents<Light>();

		for (const auto &light : sceneLights)
		{
		//	auto position = *light->GetPosition();
		//	float radius = light->GetRadius();

		//	if (radius >= 0.0f && !camera.GetViewFrustum()->SphereInFrustum(position, radius))
		//	{
		//		continue;
		//	}

			DeferredLight deferredLight = {};
			deferredLight.m_colour = light->GetColour();
			deferredLight.m_position = light->GetWorldTransform().GetPosition();
			deferredLight.m_radius = light->GetRadius();
			deferredLights[lightCount] = deferredLight;
			lightCount++;

			if (lightCount >= MAX_LIGHTS)
			{
				break;
			}
		}

		// Updates uniforms.
		m_uniformScene.Push("view", camera->GetViewMatrix());
		m_uniformScene.Push("shadowSpace", Shadows::Get()->GetShadowBox().GetToShadowMapSpaceMatrix());
		m_uniformScene.Push("cameraPosition", camera->GetPosition());

		m_uniformScene.Push("lightsCount", lightCount);

		m_uniformScene.Push("fogColour", m_fog.GetColour());
		m_uniformScene.Push("fogDensity", m_fog.GetDensity());
		m_uniformScene.Push("fogGradient", m_fog.GetGradient());

		m_uniformScene.Push("shadowDistance", Shadows::Get()->GetShadowBoxDistance());
		m_uniformScene.Push("shadowTransition", Shadows::Get()->GetShadowTransition());
		m_uniformScene.Push("shadowBias", Shadows::Get()->GetShadowBias());
		m_uniformScene.Push("shadowDarkness", Shadows::Get()->GetShadowDarkness());
		m_uniformScene.Push("shadowPCF", Shadows::Get()->GetShadowPcf());

		// Updates storage buffers.
		m_storageLights.Push(deferredLights.data(), sizeof(DeferredLight) * MAX_LIGHTS);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", m_uniformScene);
		m_descriptorSet.Push("Lights", m_storageLights);
		m_descriptorSet.Push("samplerPosition", Renderer::Get()->GetAttachment("position"));
		m_descriptorSet.Push("samplerDiffuse", Renderer::Get()->GetAttachment("diffuse"));
		m_descriptorSet.Push("samplerNormal", Renderer::Get()->GetAttachment("normal"));
		m_descriptorSet.Push("samplerMaterial", Renderer::Get()->GetAttachment("material"));
		m_descriptorSet.Push("samplerShadows", Renderer::Get()->GetAttachment("shadows"));
		m_descriptorSet.Push("samplerBrdf", m_brdf);
		m_descriptorSet.Push("samplerIbl", m_skybox); // m_ibl

		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
		m_model->CmdRender(commandBuffer);
	}

	std::vector<Shader::Define> RendererDeferred::GetDefines()
	{
		std::vector<Shader::Define> result = {};
		result.emplace_back("USE_IBL", String::To<int32_t>(m_type == Type::Ibl));
		result.emplace_back("MAX_LIGHTS", String::To(MAX_LIGHTS));
		return result;
	}

	std::shared_ptr<Texture> RendererDeferred::ComputeBrdf(const uint32_t &size)
	{
		auto result = std::make_shared<Texture>(size, size);

		// Creates the pipeline.
		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
		PipelineCompute compute = PipelineCompute("Shaders/Brdf.comp", size, size, 16);

		// Bind the pipeline.
		compute.BindPipeline(commandBuffer);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
		descriptorSet.Push("outColour", result);
		descriptorSet.Update(compute);

		// Runs the compute pipeline.
		descriptorSet.BindDescriptor(commandBuffer, compute);
		compute.CmdRender(commandBuffer);
		commandBuffer.End();
		commandBuffer.SubmitIdle();

#if defined(ACID_VERBOSE)
		// Saves the brdf texture.
		std::string filename = FileSystem::GetWorkingDirectory() + "/Brdf.png";
		FileSystem::ClearFile(filename);
		std::unique_ptr<uint8_t[]> pixels(result->GetPixels());
		Texture::WritePixels(filename, pixels.get(), result->GetWidth(), result->GetHeight(), result->GetComponents());
#endif

		return result;
	}

	std::shared_ptr<Cubemap> RendererDeferred::ComputeIbl(const std::shared_ptr<Cubemap> &source)
	{
		if (source == nullptr)
		{
			return nullptr;
		}

		Texture convolution = Texture(source->GetWidth(), source->GetHeight() * 6);

		// Creates the pipeline.
		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
		PipelineCompute compute = PipelineCompute("Shaders/Ibl.comp", source->GetWidth(), source->GetHeight(), 16);

		// Bind the pipeline.
		compute.BindPipeline(commandBuffer);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
		descriptorSet.Push("outColour", convolution);
		descriptorSet.Push("samplerColour", source);
		descriptorSet.Update(compute);

		// Runs the compute pipeline.
		descriptorSet.BindDescriptor(commandBuffer, compute);
		compute.CmdRender(commandBuffer);
		commandBuffer.End();
		commandBuffer.SubmitIdle();

#if defined(ACID_VERBOSE)
		// Saves the ibl texture.
		std::string filename = FileSystem::GetWorkingDirectory() + "/IBL.png";
		FileSystem::ClearFile(filename);
		std::unique_ptr<uint8_t[]> pixels(convolution.GetPixels());
		Texture::WritePixels(filename, pixels.get(), convolution.GetWidth(), convolution.GetHeight(), convolution.GetComponents());
#endif

		return std::make_shared<Cubemap>(source->GetWidth(), source->GetHeight(), convolution.GetPixels(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLE_COUNT_1_BIT, true, true);
	}
}
