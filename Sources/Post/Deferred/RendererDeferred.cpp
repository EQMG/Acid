#include "RendererDeferred.hpp"

#include "Files/FileSystem.hpp"
#include "Lights/Light.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Models/VertexModel.hpp"
#include "Renderer/Pipelines/PipelineCompute.hpp"
#include "Renderer/Renderer.hpp"
#include "Scenes/Scenes.hpp"
#include "Skyboxes/MaterialSkybox.hpp"

namespace acid
{
	static const uint32_t MAX_LIGHTS = 32; // TODO: Make configurable.

	RendererDeferred::RendererDeferred(const Pipeline::Stage &pipelineStage) :
		RenderPipeline(pipelineStage),
		m_pipeline(pipelineStage, {"Shaders/Deferred/Deferred.vert", "Shaders/Deferred/Deferred.frag"}, {}, GetDefines()),
		m_brdf(ComputeBRDF(512)),
		m_skybox(nullptr),
		m_irradiance(nullptr),
		m_prefiltered(nullptr),
		m_fog(Colour::White, 0.001f, 2.0f, -0.1f, 0.3f)
	{
	}

	void RendererDeferred::Render(const CommandBuffer &commandBuffer)
	{
		auto camera = Scenes::Get()->GetCamera();

		auto materialSkybox = Scenes::Get()->GetStructure()->GetComponent<MaterialSkybox>();
		auto skybox = (materialSkybox == nullptr) ? nullptr : materialSkybox->GetCubemap();

		if (m_skybox != skybox)
		{
			m_skybox = skybox;
		//	m_irradiance = ComputeIrradiance(m_skybox);
		//	m_prefiltered = ComputePrefiltered(m_skybox);
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
		m_uniformScene.Push("cameraPosition", camera->GetPosition());
		m_uniformScene.Push("lightsCount", lightCount);
		m_uniformScene.Push("fogColour", m_fog.GetColour());
		m_uniformScene.Push("fogDensity", m_fog.GetDensity());
		m_uniformScene.Push("fogGradient", m_fog.GetGradient());

		// Updates storage buffers.
		m_storageLights.Push(deferredLights.data(), sizeof(DeferredLight) * MAX_LIGHTS);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", m_uniformScene);
		m_descriptorSet.Push("Lights", m_storageLights);
		m_descriptorSet.Push("samplerPosition", Renderer::Get()->GetAttachment("position"));
		m_descriptorSet.Push("samplerDiffuse", Renderer::Get()->GetAttachment("diffuse"));
		m_descriptorSet.Push("samplerNormal", Renderer::Get()->GetAttachment("normal"));
		m_descriptorSet.Push("samplerMaterial", Renderer::Get()->GetAttachment("material"));
		m_descriptorSet.Push("samplerBRDF", m_brdf.get());
	//	m_descriptorSet.Push("samplerIrradiance", m_irradiance.get());
	//	m_descriptorSet.Push("samplerPrefiltered", m_prefiltered.get());

		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
		vkCmdDraw(commandBuffer.GetCommandBuffer(), 3, 1, 0, 0);
	}

	std::vector<Shader::Define> RendererDeferred::GetDefines()
	{
		std::vector<Shader::Define> result = {};
		result.emplace_back("MAX_LIGHTS", String::To(MAX_LIGHTS));
		return result;
	}

	std::unique_ptr<Texture> RendererDeferred::ComputeBRDF(const uint32_t &size)
	{
		auto result = std::make_unique<Texture>(size, size);

		// Creates the pipeline.
		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
		PipelineCompute compute = PipelineCompute("Shaders/Brdf.comp");

		// Bind the pipeline.
		compute.BindPipeline(commandBuffer);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
		descriptorSet.Push("outColour", result.get());
		descriptorSet.Update(compute);

		// Runs the compute pipeline.
		descriptorSet.BindDescriptor(commandBuffer, compute);
		compute.CmdRender(commandBuffer, result->GetWidth(), result->GetHeight());
		commandBuffer.End();
		commandBuffer.SubmitIdle();

#if defined(ACID_VERBOSE)
		// Saves the BRDF texture.
		std::string filename = FileSystem::GetWorkingDirectory() + "/Brdf.png";
		FileSystem::ClearFile(filename);
		std::unique_ptr<uint8_t[]> pixels(result->GetPixels());
		Texture::WritePixels(filename, pixels.get(), result->GetWidth(), result->GetHeight(), result->GetComponents());
#endif

		return result;
	}

	std::unique_ptr<Cubemap> RendererDeferred::ComputeIrradiance(const std::shared_ptr<Cubemap> &source)
	{
		if (source == nullptr)
		{
			return nullptr;
		}

		Texture irradiance = Texture(source->GetWidth(), source->GetHeight() * 6);

		// Creates the pipeline.
		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
		PipelineCompute compute = PipelineCompute("Shaders/Irradiance.comp");

		// Bind the pipeline.
		compute.BindPipeline(commandBuffer);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
		descriptorSet.Push("outColour", irradiance);
		descriptorSet.Push("samplerColour", source);
		descriptorSet.Update(compute);

		// Runs the compute pipeline.
		descriptorSet.BindDescriptor(commandBuffer, compute);
		compute.CmdRender(commandBuffer, source->GetWidth(), source->GetHeight());
		commandBuffer.End();
		commandBuffer.SubmitIdle();

#if defined(ACID_VERBOSE)
		// Saves the irradiance texture.
		std::string filename = FileSystem::GetWorkingDirectory() + "/Irradiance.png";
		FileSystem::ClearFile(filename);
		std::unique_ptr<uint8_t[]> pixels(irradiance.GetPixels());
		Texture::WritePixels(filename, pixels.get(), irradiance.GetWidth(), irradiance.GetHeight(), irradiance.GetComponents());
#endif

		return std::make_unique<Cubemap>(source->GetWidth(), source->GetHeight(), irradiance.GetPixels(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLE_COUNT_1_BIT, true, true);
	}

	std::unique_ptr<Cubemap> RendererDeferred::ComputePrefiltered(const std::shared_ptr<Cubemap> &source)
	{
		if (source == nullptr)
		{
			return nullptr;
		}

		Texture prefiltered = Texture(source->GetWidth(), source->GetHeight() * 6);

		// Creates the pipeline.
		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
		PipelineCompute compute = PipelineCompute("Shaders/Prefiltered.comp");

		// Bind the pipeline.
		compute.BindPipeline(commandBuffer);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
		descriptorSet.Push("outColour", prefiltered);
		descriptorSet.Push("samplerColour", source);
		descriptorSet.Update(compute);

		// Runs the compute pipeline.
		descriptorSet.BindDescriptor(commandBuffer, compute);
		compute.CmdRender(commandBuffer, source->GetWidth(), source->GetHeight());
		commandBuffer.End();
		commandBuffer.SubmitIdle();

#if defined(ACID_VERBOSE)
		// Saves the prefiltered texture.
		std::string filename = FileSystem::GetWorkingDirectory() + "/Prefiltered.png";
		FileSystem::ClearFile(filename);
		std::unique_ptr<uint8_t[]> pixels(prefiltered.GetPixels());
		Texture::WritePixels(filename, pixels.get(), prefiltered.GetWidth(), prefiltered.GetHeight(), prefiltered.GetComponents());
#endif

		return std::make_unique<Cubemap>(source->GetWidth(), source->GetHeight(), prefiltered.GetPixels(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLE_COUNT_1_BIT, true, true);
	}
}
