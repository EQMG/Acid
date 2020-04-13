#include "SsaoFilter.hpp"

#include "Maths/Maths.hpp"
#include "Scenes/Scenes.hpp"
#include "Resources/Resources.hpp"

namespace acid {
static const uint32_t SSAO_NOISE_DIM = 4;
static const uint32_t SSAO_KERNEL_SIZE = 64;
static const float SSAO_RADIUS = 0.5f;

SsaoFilter::SsaoFilter(const Pipeline::Stage &pipelineStage) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Ssao.frag"}, GetDefines()),
	noise(Resources::Get()->GetThreadPool().Enqueue(ComputeNoise, SSAO_NOISE_DIM)),
	kernel(SSAO_KERNEL_SIZE) {
	for (uint32_t i = 0; i < SSAO_KERNEL_SIZE; ++i) {
		Vector3f sample(Maths::Random(-1.0f, 1.0f), Maths::Random(-1.0f, 1.0f), Maths::Random(0.0f, 1.0f));
		sample = sample.Normalize();
		sample *= Maths::Random(0.0f, 1.0f);
		auto scale = static_cast<float>(i) / static_cast<float>(SSAO_KERNEL_SIZE);
		scale = Maths::Lerp(0.1f, 1.0f, scale * scale);
		kernel[i] = sample * scale;
	}
}

void SsaoFilter::Render(const CommandBuffer &commandBuffer) {
	// Updates uniforms.
	auto camera = Scenes::Get()->GetCamera();
	uniformScene.Push("kernel", *kernel.data(), sizeof(Vector3f) * SSAO_KERNEL_SIZE);
	uniformScene.Push("projection", camera->GetProjectionMatrix());
	uniformScene.Push("view", camera->GetViewMatrix());
	uniformScene.Push("cameraPosition", camera->GetPosition());

	// Updates descriptors.
	descriptorSet.Push("UniformScene", uniformScene);
	descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
	descriptorSet.Push("samplerPosition", GetAttachment("samplerPosition", "position"));
	descriptorSet.Push("samplerNormal", GetAttachment("samplerNormal", "normals"));
	descriptorSet.Push("samplerNoise", *noise);

	if (!descriptorSet.Update(pipeline))
		return;

	// Draws the object.
	pipeline.BindPipeline(commandBuffer);

	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}

std::vector<Shader::Define> SsaoFilter::GetDefines() const {
	return {
		{"SSAO_KERNEL_SIZE", String::To(SSAO_KERNEL_SIZE)},
		{"SSAO_RADIUS", String::To(SSAO_RADIUS)},
		{"RANGE_CHECK", "1"}
	};
}

std::shared_ptr<Image2d> SsaoFilter::ComputeNoise(uint32_t size) {
	/*std::vector<Vector3f> ssaoNoise(size * size);

	for (uint32_t i = 0; i < size * size; i++) {
		Vector3f noise(Maths::Random(-1.0f, 1.0f), Maths::Random(-1.0f, 1.0f), 0.0f); // Vector3(float(i) / float(size * size), 0.0f, 0.0f);
		noise = noise.Normalize();
		ssaoNoise[i] = noise;
	}

	auto noiseImage = std::make_shared<Image2d>(std::make_unique<Bitmap>(ssaoNoise, Vector2ui(size)), VK_FORMAT_R32G32B32_SFLOAT,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_NEAREST);*/

#if defined(ACID_DEBUG)
	// Saves the noise Image.
	/*auto filename = "SSAO_Noise.png";
	Vector2ui extent;
	auto pixels = noiseImage->GetPixels(extent, 1);
	Image::WritePixels(filename, pixels.get(), extent);*/
#endif

	//return noiseImage;
	return nullptr;
}
}
