#include "RendererDeferred.hpp"

#include "Files/FileSystem.hpp"
#include "Lights/Light.hpp"
#include "Models/VertexDefault.hpp"
#include "Shadows/Shadows.hpp"
#include "Resources/Resources.hpp"
#include "Renderer/Pipelines/PipelineCompute.hpp"
#include "Renderer/Renderer.hpp"
#include "Scenes/Scenes.hpp"
#include "Skyboxes/MaterialSkybox.hpp"

namespace acid
{
static const uint32_t MAX_LIGHTS = 32; // TODO: Make configurable.

RendererDeferred::RendererDeferred(const Pipeline::Stage &pipelineStage) :
	RenderPipeline(pipelineStage),
	m_pipeline(pipelineStage, { "Shaders/Deferred/Deferred.vert", "Shaders/Deferred/Deferred.frag" }, {}, GetDefines(), PipelineGraphics::Mode::Polygon,
		PipelineGraphics::Depth::None),
	m_brdf(Resources::Get()->GetThreadPool().Enqueue(ComputeBRDF, 512)),
	m_skybox(nullptr),
	m_fog(Colour::White, 0.001f, 2.0f, -0.1f, 0.3f)
{
	//auto metadata = Metadata();
	//m_pipeline.GetShader()->Encode(metadata);
	//File("Shaders/Deferred.json", new Json(&metadata)).Write();
}

void RendererDeferred::Render(const CommandBuffer &commandBuffer)
{
	auto camera = Scenes::Get()->GetCamera();

	auto materialSkybox = Scenes::Get()->GetStructure()->GetComponent<MaterialSkybox>();
	auto skybox = (materialSkybox == nullptr) ? nullptr : materialSkybox->GetImage();

	if (m_skybox != skybox)
	{
		m_skybox = skybox;
		m_irradiance = Resources::Get()->GetThreadPool().Enqueue(ComputeIrradiance, m_skybox, 64);
		m_prefiltered = Resources::Get()->GetThreadPool().Enqueue(ComputePrefiltered, m_skybox, 512);
	}

	// Updates uniforms.
	std::vector<DeferredLight> deferredLights(MAX_LIGHTS);
	uint32_t lightCount = 0;

	auto sceneLights = Scenes::Get()->GetStructure()->QueryComponents<Light>();

	for (const auto &light : sceneLights)
	{
		//auto position = *light->GetPosition();
		//float radius = light->GetRadius();

		//if (radius >= 0.0f && !camera.GetViewFrustum()->SphereInFrustum(position, radius))
		//{
		//	continue;
		//}

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

	// Updates storage buffers.
	m_storageLights.Push(deferredLights.data(), sizeof(DeferredLight) * MAX_LIGHTS);

	// Updates descriptors.
	m_descriptorSet.Push("UniformScene", m_uniformScene);
	m_descriptorSet.Push("BufferLights", m_storageLights);
	m_descriptorSet.Push("samplerShadows", Renderer::Get()->GetAttachment("shadows"));
	m_descriptorSet.Push("samplerPosition", Renderer::Get()->GetAttachment("position"));
	m_descriptorSet.Push("samplerDiffuse", Renderer::Get()->GetAttachment("diffuse"));
	m_descriptorSet.Push("samplerNormal", Renderer::Get()->GetAttachment("normal"));
	m_descriptorSet.Push("samplerMaterial", Renderer::Get()->GetAttachment("material"));
	m_descriptorSet.Push("samplerBRDF", m_brdf.Get());
	m_descriptorSet.Push("samplerIrradiance", m_irradiance.Get());
	m_descriptorSet.Push("samplerPrefiltered", m_prefiltered.Get());

	bool updateSuccess = m_descriptorSet.Update(m_pipeline);

	if (!updateSuccess)
	{
		return;
	}

	// Draws the object.
	m_pipeline.BindPipeline(commandBuffer);

	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}

std::vector<Shader::Define> RendererDeferred::GetDefines()
{
	std::vector<Shader::Define> defines;
	defines.emplace_back("MAX_LIGHTS", String::To(MAX_LIGHTS));
	return defines;
}

std::unique_ptr<Image2d> RendererDeferred::ComputeBRDF(const uint32_t &size)
{
	auto brdfImage = std::make_unique<Image2d>(size, size, nullptr, VK_FORMAT_R16G16_SFLOAT, VK_IMAGE_LAYOUT_GENERAL);

	// Creates the pipeline.
	CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
	PipelineCompute compute = PipelineCompute("Shaders/Brdf.comp");

	// Bind the pipeline.
	compute.BindPipeline(commandBuffer);

	// Updates descriptors.
	DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
	descriptorSet.Push("outColour", brdfImage.get());
	descriptorSet.Update(compute);

	// Runs the compute pipeline.
	descriptorSet.BindDescriptor(commandBuffer, compute);
	compute.CmdRender(commandBuffer, brdfImage->GetWidth(), brdfImage->GetHeight());
	commandBuffer.SubmitIdle();

#if defined(ACID_VERBOSE)
	// Saves the BRDF Image.
	/*Resources::Get()->GetThreadPool().Enqueue([](Image2d *image)
	{
		std::string filename = FileSystem::GetWorkingDirectory() + "/Brdf.png";
		FileSystem::ClearFile(filename);
		VkExtent3D extent = {};
		auto pixels = image->GetPixels(extent);
		Image::WritePixels(filename, pixels.get(), extent.width, extent.height);
	}, brdfImage.get());*/
#endif

	return brdfImage;
}

std::unique_ptr<ImageCube> RendererDeferred::ComputeIrradiance(const std::shared_ptr<ImageCube> &source, const uint32_t &size)
{
	if (source == nullptr)
	{
		return nullptr;
	}

	auto irradianceCubemap = std::make_unique<ImageCube>(size, size, nullptr, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_LAYOUT_GENERAL);

	// Creates the pipeline.
	CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
	PipelineCompute compute = PipelineCompute("Shaders/Irradiance.comp");

	// Bind the pipeline.
	compute.BindPipeline(commandBuffer);

	// Updates descriptors.
	DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
	descriptorSet.Push("outColour", irradianceCubemap.get());
	descriptorSet.Push("samplerColour", source);
	descriptorSet.Update(compute);

	// Runs the compute pipeline.
	descriptorSet.BindDescriptor(commandBuffer, compute);
	compute.CmdRender(commandBuffer, irradianceCubemap->GetWidth(), irradianceCubemap->GetHeight());
	commandBuffer.SubmitIdle();

#if defined(ACID_VERBOSE)
	// Saves the irradiance Image.
	/*Resources::Get()->GetThreadPool().Enqueue([](ImageCube *image)
	{
		std::string filename = FileSystem::GetWorkingDirectory() + "/Irradiance.png";
		FileSystem::ClearFile(filename);
		VkExtent3D extent = {};
		auto pixels = image->GetPixels(extent);
		Image::WritePixels(filename, pixels.get(), extent.width, extent.height);
	}, irradianceCubemap.get());*/
#endif

	return irradianceCubemap;
}

std::unique_ptr<ImageCube> RendererDeferred::ComputePrefiltered(const std::shared_ptr<ImageCube> &source, const uint32_t &size)
{
	if (source == nullptr)
	{
		return nullptr;
	}

	auto logicalDevice = Renderer::Get()->GetLogicalDevice();

	auto prefilteredCubemap = std::make_unique<ImageCube>(size, size, nullptr, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_GENERAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLE_COUNT_1_BIT, true, true);

	// Creates the pipeline.
	CommandBuffer commandBuffer = CommandBuffer(true, VK_QUEUE_COMPUTE_BIT);
	PipelineCompute compute = PipelineCompute("Shaders/Prefiltered.comp");

	DescriptorsHandler descriptorSet = DescriptorsHandler(compute);
	PushHandler pushHandler = PushHandler(*compute.GetShader()->GetUniformBlock("PushObject"));

	// TODO: Use image barriers between rendering (single command buffer), rework write descriptor passing. Image class also needs a restructure.
	for (uint32_t i = 0; i < prefilteredCubemap->GetMipLevels(); i++)
	{
		VkImageView levelView = VK_NULL_HANDLE;
		Image::CreateImageView(prefilteredCubemap->GetImage(), levelView, VK_IMAGE_VIEW_TYPE_CUBE, prefilteredCubemap->GetFormat(), VK_IMAGE_ASPECT_COLOR_BIT, 1, i, 6, 0);

		commandBuffer.Begin();
		compute.BindPipeline(commandBuffer);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.sampler = prefilteredCubemap->GetSampler();
		imageInfo.imageView = levelView;
		imageInfo.imageLayout = prefilteredCubemap->GetLayout();

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = VK_NULL_HANDLE; // Will be set in the descriptor handler.
		descriptorWrite.dstBinding = *compute.GetShader()->GetDescriptorLocation("outColour");
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = *compute.GetShader()->GetDescriptorType(descriptorWrite.dstBinding);
		//descriptorWrite.pImageInfo = &imageInfo;
		auto writeDescriptorSet = WriteDescriptorSet(descriptorWrite, imageInfo);

		pushHandler.Push("roughness", static_cast<float>(i) / static_cast<float>(prefilteredCubemap->GetMipLevels() - 1));

		descriptorSet.Push("PushObject", pushHandler);
		descriptorSet.Push("outColour", prefilteredCubemap.get(), std::move(writeDescriptorSet));
		descriptorSet.Push("samplerColour", source);
		descriptorSet.Update(compute);

		descriptorSet.BindDescriptor(commandBuffer, compute);
		pushHandler.BindPush(commandBuffer, compute);
		compute.CmdRender(commandBuffer, prefilteredCubemap->GetWidth() >> i, prefilteredCubemap->GetHeight() >> i);
		commandBuffer.SubmitIdle();

		vkDestroyImageView(*logicalDevice, levelView, nullptr);
	}

#if defined(ACID_VERBOSE)
	/*for (uint32_t i = 0; i < prefilteredCubemap->GetMipLevels(); i++)
	{
		// Saves the prefiltered Image.
		Resources::Get()->GetThreadPool().Enqueue([](ImageCube *image, uint32_t i)
		{
			std::string filename = FileSystem::GetWorkingDirectory() + "/Prefiltered_" + String::To(i) + ".png";
			FileSystem::ClearFile(filename);
			VkExtent3D extent = {};
			auto pixels = image->GetPixels(extent, i);
			Image::WritePixels(filename, pixels.get(), extent.width, extent.height);
		}, prefilteredCubemap.get(), i);
	}*/
#endif

	return prefilteredCubemap;
}
}
