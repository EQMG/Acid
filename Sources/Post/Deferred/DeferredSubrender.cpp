#include "DeferredSubrender.hpp"

#include "Files/File.hpp"
#include "Files/Json/Json.hpp"
#include "Lights/Light.hpp"
#include "Models/Vertex3d.hpp"
#include "Shadows/Shadows.hpp"
#include "Resources/Resources.hpp"
#include "Graphics/Pipelines/PipelineCompute.hpp"
#include "Graphics/Graphics.hpp"
#include "Scenes/Scenes.hpp"
#include "Skyboxes/SkyboxMaterial.hpp"
#include "Meshes/Mesh.hpp"

namespace acid {
static const uint32_t MAX_LIGHTS = 32; // TODO: Make configurable.

DeferredSubrender::DeferredSubrender(const Pipeline::Stage &pipelineStage) :
	Subrender(pipelineStage),
	pipeline(pipelineStage, {"Shaders/Deferred/Deferred.vert", "Shaders/Deferred/Deferred.frag"}, {}, {},
		PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None),
	brdf(Resources::Get()->GetThreadPool().Enqueue(ComputeBRDF, 512)),
	fog(Colour::White, 0.001f, 2.0f, -0.1f, 0.3f) {
#if defined(ACID_DEBUG)
	Node node;
	node << *pipeline.GetShader();
	File("Deferred/Shader.json", std::make_unique<Json>(), node).Write(NodeFormat::Beautified);
#endif
}

void DeferredSubrender::Render(const CommandBuffer &commandBuffer) {
	auto camera = Scenes::Get()->GetCamera();

	// TODO probably use a cubemap image directly instead of scene components.
	std::shared_ptr<ImageCube> skybox = nullptr;
	auto meshes = Scenes::Get()->GetStructure()->QueryComponents<Mesh>();
	for (const auto &mesh : meshes) {
		if (auto materialSkybox = dynamic_cast<const SkyboxMaterial *>(mesh->GetMaterial())) {
			skybox = materialSkybox->GetImage();
			break;
		}
	}

	if (this->skybox != skybox) {
		this->skybox = skybox;
		irradiance = Resources::Get()->GetThreadPool().Enqueue(ComputeIrradiance, skybox, 64);
		prefiltered = Resources::Get()->GetThreadPool().Enqueue(ComputePrefiltered, skybox, 512);
	}

	// Updates uniforms.
	std::vector<DeferredLight> deferredLights(MAX_LIGHTS);
	uint32_t lightCount = 0;

	auto sceneLights = Scenes::Get()->GetStructure()->QueryComponents<Light>();

	for (const auto &light : sceneLights) {
		//auto position = *light->GetPosition();
		//float radius = light->GetRadius();

		//if (radius >= 0.0f && !camera.GetViewFrustum()->SphereInFrustum(position, radius))
		//{
		//	continue;
		//}

		DeferredLight deferredLight = {};
		deferredLight.colour = light->GetColour();

		if (auto transform = light->GetEntity()->GetComponent<Transform>())
			deferredLight.position = transform->GetPosition();

		deferredLight.radius = light->GetRadius();
		deferredLights[lightCount] = deferredLight;
		lightCount++;

		if (lightCount >= MAX_LIGHTS)
			break;
	}

	// Updates uniforms.
	uniformScene.Push("view", camera->GetViewMatrix());
	uniformScene.Push("shadowSpace", Shadows::Get()->GetShadowBox().GetToShadowMapSpaceMatrix());
	uniformScene.Push("cameraPosition", camera->GetPosition());
	uniformScene.Push("lightsCount", lightCount);
	uniformScene.Push("fogColour", fog.GetColour());
	uniformScene.Push("fogDensity", fog.GetDensity());
	uniformScene.Push("fogGradient", fog.GetGradient());

	// Updates storage buffers.
	storageLights.Push(deferredLights.data(), sizeof(DeferredLight) * MAX_LIGHTS);

	// Updates descriptors.
	descriptorSet.Push("UniformScene", uniformScene);
	descriptorSet.Push("BufferLights", storageLights);
	descriptorSet.Push("samplerShadows", Graphics::Get()->GetAttachment("shadows"));
	descriptorSet.Push("samplerPosition", Graphics::Get()->GetAttachment("position"));
	descriptorSet.Push("samplerDiffuse", Graphics::Get()->GetAttachment("diffuse"));
	descriptorSet.Push("samplerNormal", Graphics::Get()->GetAttachment("normal"));
	descriptorSet.Push("samplerMaterial", Graphics::Get()->GetAttachment("material"));
	descriptorSet.Push("samplerBRDF", *brdf);
	descriptorSet.Push("samplerIrradiance", *irradiance);
	descriptorSet.Push("samplerPrefiltered", *prefiltered);

	if (!descriptorSet.Update(pipeline))
		return;

	// Draws the object.
	pipeline.BindPipeline(commandBuffer);

	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}

std::unique_ptr<Image2d> DeferredSubrender::ComputeBRDF(uint32_t size) {
	auto brdfImage = std::make_unique<Image2d>(Vector2ui(size), VK_FORMAT_R16G16_SFLOAT, VK_IMAGE_LAYOUT_GENERAL);

	// Creates the pipeline.
	CommandBuffer commandBuffer(true, VK_QUEUE_COMPUTE_BIT);
	PipelineCompute compute("Shaders/Brdf.comp");

	// Bind the pipeline.
	compute.BindPipeline(commandBuffer);

	// Updates descriptors.
	DescriptorsHandler descriptorSet(compute);
	descriptorSet.Push("outColour", brdfImage.get());
	descriptorSet.Update(compute);

	// Runs the compute pipeline.
	descriptorSet.BindDescriptor(commandBuffer, compute);
	compute.CmdRender(commandBuffer, brdfImage->GetSize());
	commandBuffer.SubmitIdle();

/*#if defined(ACID_DEBUG)
	// Saves the BRDF Image.
	Resources::Get()->GetThreadPool().Enqueue([](Image2d *image) {
		image->GetBitmap()->Write("Deferred/Brdf.png");
	}, brdfImage.get());
#endif*/

	return brdfImage;
}

std::unique_ptr<ImageCube> DeferredSubrender::ComputeIrradiance(const std::shared_ptr<ImageCube> &source, uint32_t size) {
	if (!source) {
		return nullptr;
	}

	auto irradianceCubemap = std::make_unique<ImageCube>(Vector2ui(size), VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_LAYOUT_GENERAL);

	// Creates the pipeline.
	CommandBuffer commandBuffer(true, VK_QUEUE_COMPUTE_BIT);
	PipelineCompute compute("Shaders/Irradiance.comp");

	// Bind the pipeline.
	compute.BindPipeline(commandBuffer);

	// Updates descriptors.
	DescriptorsHandler descriptorSet(compute);
	descriptorSet.Push("outColour", irradianceCubemap.get());
	descriptorSet.Push("samplerColour", source);
	descriptorSet.Update(compute);

	// Runs the compute pipeline.
	descriptorSet.BindDescriptor(commandBuffer, compute);
	compute.CmdRender(commandBuffer, irradianceCubemap->GetSize());
	commandBuffer.SubmitIdle();

/*#if defined(ACID_DEBUG)
	// Saves the irradiance Image.
	Resources::Get()->GetThreadPool().Enqueue([](ImageCube *image) {
		image->GetBitmap()->Write("Deferred/Irradiance.png");
	}, irradianceCubemap.get());
#endif*/

	return irradianceCubemap;
}

std::unique_ptr<ImageCube> DeferredSubrender::ComputePrefiltered(const std::shared_ptr<ImageCube> &source, uint32_t size) {
	if (!source) {
		return nullptr;
	}

	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	auto prefilteredCubemap = std::make_unique<ImageCube>(Vector2ui(size), VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_GENERAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLE_COUNT_1_BIT, true, true);

	// Creates the pipeline.
	CommandBuffer commandBuffer(true, VK_QUEUE_COMPUTE_BIT);
	PipelineCompute compute("Shaders/Prefiltered.comp");
	
	DescriptorsHandler descriptorSet(compute);
	PushHandler pushHandler(*compute.GetShader()->GetUniformBlock("PushObject"));

	// TODO: Use image barriers between rendering (single command buffer), rework write descriptor passing. Image class also needs a restructure.
	for (uint32_t i = 0; i < prefilteredCubemap->GetMipLevels(); i++) {
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
		WriteDescriptorSet writeDescriptorSet(descriptorWrite, imageInfo);

		pushHandler.Push("roughness", static_cast<float>(i) / static_cast<float>(prefilteredCubemap->GetMipLevels() - 1));

		descriptorSet.Push("PushObject", pushHandler);
		descriptorSet.Push("outColour", prefilteredCubemap.get(), std::move(writeDescriptorSet));
		descriptorSet.Push("samplerColour", source);
		descriptorSet.Update(compute);

		descriptorSet.BindDescriptor(commandBuffer, compute);
		pushHandler.BindPush(commandBuffer, compute);
		compute.CmdRender(commandBuffer, prefilteredCubemap->GetSize() >> i);
		commandBuffer.SubmitIdle();

		vkDestroyImageView(*logicalDevice, levelView, nullptr);
	}

	// TODO: This debug write causes a crash at runtime, why?
/*#if defined(ACID_DEBUG)
	// Saves the prefiltered Image.
	Resources::Get()->GetThreadPool().Enqueue([](ImageCube *image) {
		for (uint32_t i = 0; i < image->GetMipLevels(); i++) {
			image->GetBitmap(i)->Write("Deferred/Prefiltered_" + String::To(i) + ".png");
		}
	}, prefilteredCubemap.get());
#endif*/

	return prefilteredCubemap;
}
}
