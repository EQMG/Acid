#include <Helpers/FileSystem.hpp>
#include "FilterSsao.hpp"

#include "Renderer/Swapchain/DepthStencil.hpp"
#include "Maths/Colour.hpp"
#include "Maths/Maths.hpp"

namespace acid
{
	const uint32_t FilterSsao::SSAO_NOISE_DIM = 4;
	const uint32_t FilterSsao::SSAO_KERNEL_SIZE = 64;
	const float FilterSsao::SSAO_RADIUS = 0.5f;

	FilterSsao::FilterSsao(const GraphicsStage &graphicsStage) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Ssao.frag"}, GetDefines()),
		m_uniformScene(UniformHandler()),
		m_noise(ComputeNoise(SSAO_NOISE_DIM)),
		m_kernel(std::vector<Vector3>(SSAO_KERNEL_SIZE))
	{
		for (uint32_t i = 0; i < SSAO_KERNEL_SIZE; ++i)
		{
			Vector3 sample = Vector3(Maths::Random(-1.0f, 1.0f), Maths::Random(-1.0f, 1.0f), Maths::Random(0.0f, 1.0f));
			sample = sample.Normalize();
			sample *= Maths::Random(0.0f, 1.0f);
			float scale = static_cast<float>(i) / static_cast<float>(SSAO_KERNEL_SIZE);
			scale = Maths::Lerp(0.1f, 1.0f, scale * scale);
			m_kernel[i] = Vector4(sample * scale, 0.0f);
		}
	}

	void FilterSsao::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("kernel", *m_kernel.data(), sizeof(Vector3) * SSAO_KERNEL_SIZE);
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());
		m_uniformScene.Push("nearPlane", camera.GetNearPlane());
		m_uniformScene.Push("farPlane", camera.GetFarPlane());

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(5));
		m_descriptorSet.Push("samplerDepth", m_pipeline.GetDepthStencil());
		m_descriptorSet.Push("samplerNormal", m_pipeline.GetTexture(3));
		m_descriptorSet.Push("samplerNoise", m_noise);
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

	std::vector<PipelineDefine> FilterSsao::GetDefines()
	{
		std::vector<PipelineDefine> result = {};
		result.emplace_back(PipelineDefine("SSAO_KERNEL_SIZE", String::To(SSAO_KERNEL_SIZE)));
		result.emplace_back(PipelineDefine("SSAO_RADIUS", String::To(SSAO_RADIUS)));
		return result;
	}

	std::shared_ptr<acid::Texture> FilterSsao::ComputeNoise(const uint32_t &size)
	{
		auto ssaoNoise = std::vector<Colour>(size * size);

		for (uint32_t i = 0; i < size * size; i++)
		{
			Vector3 noise = Vector3(float(i) / float(size * size), 0.0f, 0.0f); // Vector3(Maths::Random(-1.0f, 1.0f), Maths::Random(-1.0f, 1.0f), 0.0f);
			noise = noise.Normalize();
			ssaoNoise[i] = Colour(noise, 1.0f);
		}

		return std::make_shared<Texture>(size, size, ssaoNoise.data(), VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_FILTER_NEAREST);
	}
}
