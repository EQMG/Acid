#pragma once

#include "Helpers/Future.hpp"
#include "Lights/Fog.hpp"
#include "Maths/Vector3.hpp"
#include "Graphics/Subrender.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Images/ImageCube.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT SubrenderDeferred :
	public Subrender
{
public:
	explicit SubrenderDeferred(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

	const Fog &GetFog() const { return m_fog; }

	void SetFog(const Fog &fog) { m_fog = fog; }

	static std::unique_ptr<Image2d> ComputeBRDF(uint32_t size);

	static std::unique_ptr<ImageCube> ComputeIrradiance(const std::shared_ptr<ImageCube> &source, uint32_t size);

	static std::unique_ptr<ImageCube> ComputePrefiltered(const std::shared_ptr<ImageCube> &source, uint32_t size);

private:
	struct DeferredLight
	{
		Colour m_colour{};
		Vector3f m_position{};
		float m_radius{};
	};

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
