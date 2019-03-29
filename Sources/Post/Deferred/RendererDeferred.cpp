#include "RendererDeferred.hpp"

#include "Files/FileSystem.hpp"
#include "Lights/Light.hpp"
#include "Models/VertexModel.hpp"
#include "Resources/Resources.hpp"
#include "Renderer/Pipelines/PipelineCompute.hpp"
#include "Renderer/Renderer.hpp"
#include "Scenes/Scenes.hpp"
#include "Skyboxes/MaterialSkybox.hpp"
#include "Files/File.hpp"
#include "Serialized/Json/Json.hpp"

namespace acid
{
	static const uint32_t MAX_LIGHTS = 32; // TODO: Make configurable.

	RendererDeferred::RendererDeferred(const Pipeline::Stage &pipelineStage) :
		RenderPipeline(pipelineStage),
		m_pipeline(pipelineStage, {"Shaders/Deferred/Deferred.vert", "Shaders/Deferred/Deferred.frag"}, {}, GetDefines(), 
			PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None),
		m_brdfFuture(Resources::Get()->GetThreadPool().Enqueue(RendererDeferred::ComputeBRDF, 512)),
		m_skybox(nullptr),
		m_irradiance(nullptr),
		m_prefiltered(nullptr),
		m_fog(Colour::White, 0.001f, 2.0f, -0.1f, 0.3f)
	{
	//	auto metadata = Metadata();
	//	m_pipeline.GetShader()->Encode(metadata);
	//	File("Shaders/Deferred.json", new Json(&metadata)).Write();
	}

	void RendererDeferred::Render(const CommandBuffer &commandBuffer)
	{
		auto camera = Scenes::Get()->GetCamera();

		auto materialSkybox = Scenes::Get()->GetStructure()->GetComponent<MaterialSkybox>();
		auto skybox = (materialSkybox == nullptr) ? nullptr : materialSkybox->GetCubemap();

		if (m_skybox != skybox)
		{
			m_skybox = skybox;
			m_irradianceFuture = Resources::Get()->GetThreadPool().Enqueue(RendererDeferred::ComputeIrradiance, m_skybox, 64);
			m_prefilteredFuture = Resources::Get()->GetThreadPool().Enqueue(RendererDeferred::ComputePrefiltered, m_skybox, 512);
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

		if (m_brdfFuture.valid())
		{
			m_brdf = m_brdfFuture.get();
		}
		if (m_irradianceFuture.valid())
		{
			m_irradiance = m_irradianceFuture.get();
		}
		if (m_prefilteredFuture.valid())
		{
			m_prefiltered = m_prefilteredFuture.get();
		}

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", m_uniformScene);
		m_descriptorSet.Push("Lights", m_storageLights);
		m_descriptorSet.Push("samplerPosition", Renderer::Get()->GetAttachment("position"));
		m_descriptorSet.Push("samplerDiffuse", Renderer::Get()->GetAttachment("diffuse"));
		m_descriptorSet.Push("samplerNormal", Renderer::Get()->GetAttachment("normal"));
		m_descriptorSet.Push("samplerMaterial", Renderer::Get()->GetAttachment("material"));
		m_descriptorSet.Push("samplerBRDF", m_brdf.get());
		m_descriptorSet.Push("samplerIrradiance", m_irradiance.get());
		m_descriptorSet.Push("samplerPrefiltered", m_prefiltered.get());

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

	std::unique_ptr<Image2d> RendererDeferred::ComputeBRDF(const uint32_t &size)
	{
		auto result = std::make_unique<Image2d>(size, size);

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
		Resources::Get()->GetThreadPool().Enqueue([](Image2d *result)
		{
			std::string filename = FileSystem::GetWorkingDirectory() + "/Brdf.png";
			FileSystem::ClearFile(filename);
			uint32_t width = 0;
			uint32_t height = 0;
			auto pixels = result->GetPixels(width, height, 1);
			Image::WritePixels(filename, pixels.get(), width, height);
		}, result.get());
#endif

		return result;
	}

	std::unique_ptr<ImageCube> RendererDeferred::ComputeIrradiance(const std::shared_ptr<ImageCube> &source, const uint32_t &size)
	{
		if (source == nullptr)
		{
			return nullptr;
		}

		auto result = std::make_unique<ImageCube>(size, size);

		// Creates the pipeline.
		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
		PipelineCompute compute = PipelineCompute("Shaders/Irradiance.comp");

		// Bind the pipeline.
		compute.BindPipeline(commandBuffer);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
		descriptorSet.Push("outColour", result.get());
		descriptorSet.Push("samplerColour", source);
		descriptorSet.Update(compute);

		// Runs the compute pipeline.
		descriptorSet.BindDescriptor(commandBuffer, compute);
		compute.CmdRender(commandBuffer, result->GetWidth(), result->GetHeight());
		commandBuffer.End();
		commandBuffer.SubmitIdle();

#if defined(ACID_VERBOSE)
		// Saves the irradiance texture.
		Resources::Get()->GetThreadPool().Enqueue([](ImageCube *result)
		{
			std::string filename = FileSystem::GetWorkingDirectory() + "/Irradiance.png";
			FileSystem::ClearFile(filename);
			uint32_t width = 0;
			uint32_t height = 0;
			auto pixels = result->GetPixels(width, height, 1);
			Image::WritePixels(filename, pixels.get(), width, height);
		}, result.get());
#endif

		return result;
	}

	std::unique_ptr<ImageCube> RendererDeferred::ComputePrefiltered(const std::shared_ptr<ImageCube> &source, const uint32_t &size)
	{
		if (source == nullptr)
		{
			return nullptr;
		}

		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		auto result = std::make_unique<ImageCube>(size, size, nullptr, VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, 
			VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLE_COUNT_1_BIT, true, true);

		// Creates the pipeline.
		CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
		PipelineCompute compute = PipelineCompute("Shaders/Prefiltered.comp");

		for (uint32_t i = 0; i < result->GetMipLevels(); i++)
		{
			VkImageView levelView = VK_NULL_HANDLE;
			Image::CreateImageView(result->GetImage(), levelView, VK_IMAGE_VIEW_TYPE_2D, result->GetFormat(), VK_IMAGE_ASPECT_COLOR_BIT, 1, i, 1, 0);
			// static_cast<float>(i) / static_cast<float>(result->GetMipLevels())
			vkDestroyImageView(logicalDevice->GetLogicalDevice(), levelView, nullptr);
		}


	//	PushHandler pushHandler = PushHandler(*compute.GetShader()->GetUniformBlock("PushObject"));
	//	pushHandler.Push("roughness", 0.6f);

		// Updates descriptors.
		DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
	//	descriptorSet.Push("PushObject", pushHandler);
		descriptorSet.Push("outColour", result.get());
		descriptorSet.Push("samplerColour", source);
		descriptorSet.Update(compute);

		// Runs the compute pipeline.
	//	pushHandler.BindPush(commandBuffer, compute);
		compute.BindPipeline(commandBuffer);

		descriptorSet.BindDescriptor(commandBuffer, compute);
		compute.CmdRender(commandBuffer, result->GetWidth(), result->GetHeight());

		commandBuffer.End();
		commandBuffer.SubmitIdle();

#if defined(ACID_VERBOSE)
		// Saves the prefiltered texture.
		Resources::Get()->GetThreadPool().Enqueue([](ImageCube *result)
		{
			std::string filename = FileSystem::GetWorkingDirectory() + "/Prefiltered.png";
			FileSystem::ClearFile(filename);
			uint32_t width = 0;
			uint32_t height = 0;
			auto pixels = result->GetPixels(width, height, 1);
			Image::WritePixels(filename, pixels.get(), width, height);

			/*for (uint32_t i = 1; i < result->GetMipLevels() + 1; i++)
			{
				std::string filename = FileSystem::GetWorkingDirectory() + "/Prefiltered_" + String::To(i) + ".png";
				FileSystem::ClearFile(filename);
				uint32_t width = 0;
				uint32_t height = 0;
				auto pixels = result->GetPixels(width, height, i);
				Image::WritePixels(filename, pixels.get(), width, height);
			}*/
		}, result.get());
#endif

		return result;
	}
}
