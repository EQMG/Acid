#include "FilterSsao.hpp"

#include "Files/FileSystem.hpp"
#include "Maths/Colour.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Maths.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
static const uint32_t SSAO_NOISE_DIM = 4;
static const uint32_t SSAO_KERNEL_SIZE = 64;
static const float SSAO_RADIUS = 0.5f;

FilterSsao::FilterSsao(const Pipeline::Stage &pipelineStage) :
	PostFilter(pipelineStage, { "Shaders/Post/Default.vert", "Shaders/Post/Ssao.frag" }, GetDefines()),
	m_noise(ComputeNoise(SSAO_NOISE_DIM)),
	m_kernel(SSAO_KERNEL_SIZE)
{
	for (uint32_t i = 0; i < SSAO_KERNEL_SIZE; ++i)
	{
		Vector3f sample = Vector3f(Maths::Random(-1.0f, 1.0f), Maths::Random(-1.0f, 1.0f), Maths::Random(0.0f, 1.0f));
		sample = sample.Normalize();
		sample *= Maths::Random(0.0f, 1.0f);
		float scale = static_cast<float>(i) / static_cast<float>(SSAO_KERNEL_SIZE);
		scale = Maths::Lerp(0.1f, 1.0f, scale * scale);
		m_kernel[i] = sample * scale;
	}
}

void FilterSsao::Render(const CommandBuffer &commandBuffer)
{
	// Updates uniforms.
	auto camera = Scenes::Get()->GetCamera();
	m_uniformScene.Push("kernel", *m_kernel.data(), sizeof(Vector3f) * SSAO_KERNEL_SIZE);
	m_uniformScene.Push("projection", camera->GetProjectionMatrix());
	m_uniformScene.Push("view", camera->GetViewMatrix());
	m_uniformScene.Push("cameraPosition", camera->GetPosition());

	// Updates descriptors.
	m_descriptorSet.Push("UboScene", m_uniformScene);
	m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
	m_descriptorSet.Push("samplerPosition", GetAttachment("samplerPosition", "position"));
	m_descriptorSet.Push("samplerNormal", GetAttachment("samplerNormal", "normals"));
	m_descriptorSet.Push("samplerNoise", m_noise);
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

std::vector<Shader::Define> FilterSsao::GetDefines()
{
	std::vector<Shader::Define> defines;
	defines.emplace_back("SSAO_KERNEL_SIZE", String::To(SSAO_KERNEL_SIZE));
	defines.emplace_back("SSAO_RADIUS", String::To(SSAO_RADIUS));
	defines.emplace_back("RANGE_CHECK", "1");
	return defines;
}

std::shared_ptr<Image2d> FilterSsao::ComputeNoise(const uint32_t &size)
{
	std::vector<Vector3f> ssaoNoise(size * size);

	for (uint32_t i = 0; i < size * size; i++)
	{
		Vector3f noise = Vector3f(Maths::Random(-1.0f, 1.0f), Maths::Random(-1.0f, 1.0f), 0.0f); // Vector3(float(i) / float(size * size), 0.0f, 0.0f);
		noise = noise.Normalize();
		ssaoNoise[i] = noise;
	}

	auto noiseImage = std::make_shared<Image2d>(size, size, std::unique_ptr<uint8_t[]>(reinterpret_cast<uint8_t *>(ssaoNoise.data())), VK_FORMAT_R32G32B32_SFLOAT,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_NEAREST);

#if defined(ACID_VERBOSE)
	// Saves the noise texture.
	/*std::string filename = FileSystem::GetWorkingDirectory() + "/SSAO_Noise.png";
	FileSystem::ClearFile(filename);
	uint32_t width = 0;
	uint32_t height = 0;
	auto pixels = noiseImage->GetPixels(width, height, 1);
	Image::WritePixels(filename, pixels.get(), width, height);*/
#endif

	return noiseImage;
}
}
