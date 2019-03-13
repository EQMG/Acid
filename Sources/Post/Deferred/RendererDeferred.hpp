#pragma once

#include "Lights/Fog.hpp"
#include "Maths/Vector3.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Renderer/RenderPipeline.hpp"
#include "Textures/Cubemap.hpp"

namespace acid
{
class ACID_EXPORT RendererDeferred : public RenderPipeline
{
  public:
	explicit RendererDeferred(const Pipeline::Stage& pipelineStage);

	void Render(const CommandBuffer& commandBuffer) override;

	const Fog& GetFog() const
	{
		return m_fog;
	}

	void SetFog(const Fog& fog)
	{
		m_fog = fog;
	}

  private:
	struct DeferredLight
	{
		Colour m_colour;
		Vector3 m_position;
		float m_radius{};
	};

	std::vector<Shader::Define> GetDefines();

	static std::unique_ptr<Texture> ComputeBRDF(const uint32_t& size);

	static std::unique_ptr<Cubemap> ComputeIrradiance(const std::shared_ptr<Cubemap>& source, const uint32_t& size);

	static std::unique_ptr<Cubemap> ComputePrefiltered(const std::shared_ptr<Cubemap>& source, const uint32_t& size);

	DescriptorsHandler m_descriptorSet;
	UniformHandler m_uniformScene;
	StorageHandler m_storageLights;

	PipelineGraphics m_pipeline;

	std::unique_ptr<Texture> m_brdf;

	std::shared_ptr<Cubemap> m_skybox;
	std::unique_ptr<Cubemap> m_irradiance;
	std::unique_ptr<Cubemap> m_prefiltered;

	Fog m_fog;
};
}
