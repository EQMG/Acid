#pragma once

#include "Helpers/Future.hpp"
#include "Lights/Fog.hpp"
#include "Maths/Vector3.hpp"
#include "Renderer/RenderPipeline.hpp"
#include "Renderer/Descriptors/DescriptorsHandler.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
#include "Renderer/Images/ImageCube.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT RendererDeferred :
	public RenderPipeline
{
public:
	explicit RendererDeferred(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

	const Fog &GetFog() const { return m_fog; }

	void SetFog(const Fog &fog) { m_fog = fog; }

	static std::unique_ptr<Image2d> ComputeBRDF(const uint32_t &size);

	static std::unique_ptr<ImageCube> ComputeIrradiance(const std::shared_ptr<ImageCube> &source, const uint32_t &size);

	static std::unique_ptr<ImageCube> ComputePrefiltered(const std::shared_ptr<ImageCube> &source, const uint32_t &size);

private:
	struct DeferredLight
	{
		Colour m_colour{};
		Vector3 m_position{};
		float m_radius{};
	};

	std::vector<Shader::Define> GetDefines();

	DescriptorsHandler m_descriptorSet;
	UniformHandler m_uniformScene;
	StorageHandler m_storageLights;

	PipelineGraphics m_pipeline;

	Future<std::unique_ptr<Image2d>> m_brdf;

	std::shared_ptr<ImageCube> m_skybox;

	Future<std::unique_ptr<ImageCube>> m_irradiance;
	Future<std::unique_ptr<ImageCube>> m_prefiltered;

	Fog m_fog;
};
}
